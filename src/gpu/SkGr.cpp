/*
 * Copyright 2010 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "SkGr.h"
#include "GrBitmapTextureMaker.h"
#include "GrCaps.h"
#include "GrColorSpaceXform.h"
#include "GrContext.h"
#include "GrContextPriv.h"
#include "GrGpuResourcePriv.h"
#include "GrPaint.h"
#include "GrResourceProvider.h"
#include "GrTextureProxy.h"
#include "GrTypes.h"
#include "GrXferProcessor.h"
#include "SkAutoMalloc.h"
#include "SkBlendModePriv.h"
#include "SkCanvas.h"
#include "SkColorFilter.h"
#include "SkConvertPixels.h"
#include "SkData.h"
#include "SkImageInfoPriv.h"
#include "SkMaskFilter.h"
#include "SkMessageBus.h"
#include "SkMipMap.h"
#include "SkPM4fPriv.h"
#include "SkPaintPriv.h"
#include "SkPixelRef.h"
#include "SkResourceCache.h"
#include "SkShaderBase.h"
#include "SkTemplates.h"
#include "SkTraceEvent.h"
#include "effects/GrBicubicEffect.h"
#include "effects/GrConstColorProcessor.h"
#include "effects/GrDitherEffect.h"
#include "effects/GrPorterDuffXferProcessor.h"
#include "effects/GrXfermodeFragmentProcessor.h"

GrSurfaceDesc GrImageInfoToSurfaceDesc(const SkImageInfo& info, const GrCaps& caps) {
    GrSurfaceDesc desc;
    desc.fFlags = kNone_GrSurfaceFlags;
    desc.fOrigin = kTopLeft_GrSurfaceOrigin;
    desc.fWidth = info.width();
    desc.fHeight = info.height();
    desc.fConfig = SkImageInfo2GrPixelConfig(info, caps);
    desc.fSampleCnt = 0;
    return desc;
}

void GrMakeKeyFromImageID(GrUniqueKey* key, uint32_t imageID, const SkIRect& imageBounds) {
    SkASSERT(key);
    SkASSERT(imageID);
    SkASSERT(!imageBounds.isEmpty());
    static const GrUniqueKey::Domain kImageIDDomain = GrUniqueKey::GenerateDomain();
    GrUniqueKey::Builder builder(key, kImageIDDomain, 5);
    builder[0] = imageID;
    builder[1] = imageBounds.fLeft;
    builder[2] = imageBounds.fTop;
    builder[3] = imageBounds.fRight;
    builder[4] = imageBounds.fBottom;
}

//////////////////////////////////////////////////////////////////////////////
sk_sp<GrTextureProxy> GrUploadBitmapToTextureProxy(GrResourceProvider* resourceProvider,
                                                   const SkBitmap& bitmap,
                                                   SkColorSpace* dstColorSpace) {
    if (!bitmap.readyToDraw()) {
        return nullptr;
    }
    SkPixmap pixmap;
    if (!bitmap.peekPixels(&pixmap)) {
        return nullptr;
    }
    return GrUploadPixmapToTextureProxy(resourceProvider, pixmap, SkBudgeted::kYes, dstColorSpace);
}

static const SkPixmap* compute_desc(const GrCaps& caps, const SkPixmap& pixmap,
                                    GrSurfaceDesc* desc,
                                    SkBitmap* tmpBitmap, SkPixmap* tmpPixmap) {
    const SkPixmap* pmap = &pixmap;

    *desc = GrImageInfoToSurfaceDesc(pixmap.info(), caps);

    // TODO: We're checking for srgbSupport, but we can then end up picking sBGRA as our pixel
    // config (which may not be supported). We need better fallback management here.
    SkColorSpace* colorSpace = pixmap.colorSpace();

    if (caps.srgbSupport() &&
        colorSpace && colorSpace->gammaCloseToSRGB() && !GrPixelConfigIsSRGB(desc->fConfig)) {
        // We were supplied an sRGB-like color space, but we don't have a suitable pixel config.
        // Convert to 8888 sRGB so we can handle the data correctly. The raster backend doesn't
        // handle sRGB Index8 -> sRGB 8888 correctly (yet), so lie about both the source and
        // destination (claim they're linear):
        SkImageInfo linSrcInfo = SkImageInfo::Make(pixmap.width(), pixmap.height(),
                                                   pixmap.colorType(), pixmap.alphaType());
        SkPixmap linSrcPixmap(linSrcInfo, pixmap.addr(), pixmap.rowBytes());

        SkImageInfo dstInfo = SkImageInfo::Make(pixmap.width(), pixmap.height(),
                                                kN32_SkColorType, kPremul_SkAlphaType,
                                                pixmap.info().refColorSpace());

        tmpBitmap->allocPixels(dstInfo);

        SkImageInfo linDstInfo = SkImageInfo::MakeN32Premul(pixmap.width(), pixmap.height());
        if (!linSrcPixmap.readPixels(linDstInfo, tmpBitmap->getPixels(), tmpBitmap->rowBytes())) {
            return nullptr;
        }
        if (!tmpBitmap->peekPixels(tmpPixmap)) {
            return nullptr;
        }
        pmap = tmpPixmap;
        // must rebuild desc, since we've forced the info to be N32
        *desc = GrImageInfoToSurfaceDesc(pmap->info(), caps);
    }

    return pmap;
}

sk_sp<GrTextureProxy> GrUploadPixmapToTextureProxy(GrResourceProvider* resourceProvider,
                                                   const SkPixmap& pixmap,
                                                   SkBudgeted budgeted,
                                                   SkColorSpace* dstColorSpace) {
    SkDestinationSurfaceColorMode colorMode = dstColorSpace
        ? SkDestinationSurfaceColorMode::kGammaAndColorSpaceAware
        : SkDestinationSurfaceColorMode::kLegacy;

    if (!SkImageInfoIsValid(pixmap.info(), colorMode)) {
        return nullptr;
    }

    SkBitmap tmpBitmap;
    SkPixmap tmpPixmap;
    GrSurfaceDesc desc;

    ATRACE_ANDROID_FRAMEWORK("Upload Texture [%ux%u]", pixmap.width(), pixmap.height());
    if (const SkPixmap* pmap = compute_desc(*resourceProvider->caps(), pixmap, &desc,
                                            &tmpBitmap, &tmpPixmap)) {
        return GrSurfaceProxy::MakeDeferred(resourceProvider, desc,
                                            budgeted, pmap->addr(), pmap->rowBytes());
    }

    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////

void GrInstallBitmapUniqueKeyInvalidator(const GrUniqueKey& key, SkPixelRef* pixelRef) {
    class Invalidator : public SkPixelRef::GenIDChangeListener {
    public:
        explicit Invalidator(const GrUniqueKey& key) : fMsg(key) {}
    private:
        GrUniqueKeyInvalidatedMessage fMsg;

        void onChange() override { SkMessageBus<GrUniqueKeyInvalidatedMessage>::Post(fMsg); }
    };

    pixelRef->addGenIDChangeListener(new Invalidator(key));
}

sk_sp<GrTextureProxy> GrGenerateMipMapsAndUploadToTextureProxy(GrContext* ctx,
                                                               const SkBitmap& bitmap,
                                                               SkColorSpace* dstColorSpace) {
    SkDestinationSurfaceColorMode colorMode = dstColorSpace
        ? SkDestinationSurfaceColorMode::kGammaAndColorSpaceAware
        : SkDestinationSurfaceColorMode::kLegacy;

    if (!SkImageInfoIsValid(bitmap.info(), colorMode)) {
        return nullptr;
    }

    SkPixmap pixmap;
    if (!bitmap.peekPixels(&pixmap)) {
        return nullptr;
    }

    SkBitmap tmpBitmap;
    SkPixmap tmpPixmap;
    GrSurfaceDesc desc;
    const SkPixmap* pmap = compute_desc(*ctx->resourceProvider()->caps(), pixmap, &desc,
                                        &tmpBitmap, &tmpPixmap);
    if (!pmap) {
        return nullptr;
    }

    ATRACE_ANDROID_FRAMEWORK("Upload MipMap Texture [%ux%u]", pmap->width(), pmap->height());
    std::unique_ptr<SkMipMap> mipmaps(SkMipMap::Build(*pmap, colorMode, nullptr));
    if (!mipmaps) {
        return nullptr;
    }

    const int mipLevelCount = mipmaps->countLevels() + 1;
    if (mipLevelCount < 1) {
        return nullptr;
    }

    std::unique_ptr<GrMipLevel[]> texels(new GrMipLevel[mipLevelCount]);

    texels[0].fPixels = pmap->addr();
    texels[0].fRowBytes = pmap->rowBytes();

    for (int i = 1; i < mipLevelCount; ++i) {
        SkMipMap::Level generatedMipLevel;
        mipmaps->getLevel(i - 1, &generatedMipLevel);
        texels[i].fPixels = generatedMipLevel.fPixmap.addr();
        texels[i].fRowBytes = generatedMipLevel.fPixmap.rowBytes();
    }

    return GrSurfaceProxy::MakeDeferredMipMap(ctx->resourceProvider(),
                                              desc,
                                              SkBudgeted::kYes,
                                              texels.get(),
                                              mipLevelCount,
                                              colorMode);
}

sk_sp<GrTextureProxy> GrCopyBaseMipMapToTextureProxy(GrContext* ctx,
                                                     GrTextureProxy* baseProxy) {
    SkASSERT(baseProxy);

    if (!ctx->caps()->isConfigCopyable(baseProxy->config())) {
        return nullptr;
    }

    GrSurfaceDesc desc;
    desc.fFlags = kNone_GrSurfaceFlags;
    desc.fOrigin = baseProxy->origin();
    desc.fWidth = baseProxy->width();
    desc.fHeight = baseProxy->height();
    desc.fConfig = baseProxy->config();
    desc.fSampleCnt = 0;

    sk_sp<GrTextureProxy> proxy = GrSurfaceProxy::MakeDeferredMipMap(ctx->resourceProvider(),
                                                                     desc,
                                                                     SkBudgeted::kYes);
    if (!proxy) {
        return nullptr;
    }

    // Copy the base layer to our proxy
    sk_sp<GrSurfaceContext> sContext = ctx->contextPriv().makeWrappedSurfaceContext(proxy, nullptr);
    SkASSERT(sContext);
    SkAssertResult(sContext->copy(baseProxy));

    return proxy;
}


sk_sp<GrTextureProxy> GrUploadMipMapToTextureProxy(GrContext* ctx, const SkImageInfo& info,
                                                   const GrMipLevel texels[],
                                                   int mipLevelCount,
                                                   SkDestinationSurfaceColorMode colorMode) {
    if (!SkImageInfoIsValid(info, colorMode)) {
        return nullptr;
    }

    return GrSurfaceProxy::MakeDeferredMipMap(ctx->resourceProvider(),
                                              GrImageInfoToSurfaceDesc(info, *ctx->caps()),
                                              SkBudgeted::kYes, texels,
                                              mipLevelCount, colorMode);
}

sk_sp<GrTextureProxy> GrRefCachedBitmapTextureProxy(GrContext* ctx,
                                                    const SkBitmap& bitmap,
                                                    const GrSamplerState& params,
                                                    SkScalar scaleAdjust[2]) {
    // Caller doesn't care about the texture's color space (they can always get it from the bitmap)
    return GrBitmapTextureMaker(ctx, bitmap).refTextureProxyForParams(params, nullptr,
                                                                      nullptr, scaleAdjust);
}

sk_sp<GrTextureProxy> GrMakeCachedBitmapProxy(GrResourceProvider* resourceProvider,
                                              const SkBitmap& bitmap) {
    GrUniqueKey originalKey;

    if (!bitmap.isVolatile()) {
        SkIPoint origin = bitmap.pixelRefOrigin();
        SkIRect subset = SkIRect::MakeXYWH(origin.fX, origin.fY, bitmap.width(), bitmap.height());
        GrMakeKeyFromImageID(&originalKey, bitmap.pixelRef()->getGenerationID(), subset);
    }

    sk_sp<GrTextureProxy> proxy;

    if (originalKey.isValid()) {
        proxy = resourceProvider->findOrCreateProxyByUniqueKey(originalKey,
                                                               kTopLeft_GrSurfaceOrigin);
    }
    if (!proxy) {
        // Pass nullptr for |dstColorSpace|.  This is lenient - we allow a wider range of
        // color spaces in legacy mode.  Unfortunately, we have to be lenient here, since
        // we can't necessarily know the |dstColorSpace| at this time.
        proxy = GrUploadBitmapToTextureProxy(resourceProvider, bitmap, nullptr);
        if (proxy && originalKey.isValid()) {
            SkASSERT(proxy->origin() == kTopLeft_GrSurfaceOrigin);
            resourceProvider->assignUniqueKeyToProxy(originalKey, proxy.get());
            GrInstallBitmapUniqueKeyInvalidator(originalKey, bitmap.pixelRef());
        }
    }

    return proxy;
}

///////////////////////////////////////////////////////////////////////////////

GrColor4f SkColorToPremulGrColor4f(SkColor c, const GrColorSpaceInfo& colorSpaceInfo) {
    // We want to premultiply after linearizing, so this is easy:
    return SkColorToUnpremulGrColor4f(c, colorSpaceInfo).premul();
}

GrColor4f SkColorToPremulGrColor4fLegacy(SkColor c) {
    return GrColor4f::FromGrColor(SkColorToUnpremulGrColor(c)).premul();
}

GrColor4f SkColorToUnpremulGrColor4f(SkColor c, const GrColorSpaceInfo& colorSpaceInfo) {
    GrColor4f color;
    if (colorSpaceInfo.colorSpace()) {
        // SkColor4f::FromColor does sRGB -> Linear
        color = GrColor4f::FromSkColor4f(SkColor4f::FromColor(c));
    } else {
        // GrColor4f::FromGrColor just multiplies by 1/255
        color = GrColor4f::FromGrColor(SkColorToUnpremulGrColor(c));
    }

    if (auto* xform = colorSpaceInfo.colorSpaceXformFromSRGB()) {
        color = xform->clampedXform(color);
    }

    return color;
}

///////////////////////////////////////////////////////////////////////////////

GrPixelConfig SkImageInfo2GrPixelConfig(const SkColorType type, SkColorSpace* cs,
                                        const GrCaps& caps) {
    // We intentionally ignore profile type for non-8888 formats. Anything we can't support
    // in hardware will be expanded to sRGB 8888 in GrUploadPixmapToTexture.
    switch (type) {
        case kUnknown_SkColorType:
            return kUnknown_GrPixelConfig;
        case kAlpha_8_SkColorType:
            return kAlpha_8_GrPixelConfig;
        case kRGB_565_SkColorType:
            return kRGB_565_GrPixelConfig;
        case kARGB_4444_SkColorType:
            return kRGBA_4444_GrPixelConfig;
        case kRGBA_8888_SkColorType:
            return (caps.srgbSupport() && cs && cs->gammaCloseToSRGB())
                   ? kSRGBA_8888_GrPixelConfig : kRGBA_8888_GrPixelConfig;
        case kBGRA_8888_SkColorType:
            return (caps.srgbSupport() && cs && cs->gammaCloseToSRGB())
                   ? kSBGRA_8888_GrPixelConfig : kBGRA_8888_GrPixelConfig;
        case kGray_8_SkColorType:
            return kGray_8_GrPixelConfig;
        case kRGBA_F16_SkColorType:
            return kRGBA_half_GrPixelConfig;
    }
    SkASSERT(0);    // shouldn't get here
    return kUnknown_GrPixelConfig;
}

GrPixelConfig SkImageInfo2GrPixelConfig(const SkImageInfo& info, const GrCaps& caps) {
    return SkImageInfo2GrPixelConfig(info.colorType(), info.colorSpace(), caps);
}

bool GrPixelConfigToColorType(GrPixelConfig config, SkColorType* ctOut) {
    SkColorType ct;
    switch (config) {
        case kAlpha_8_GrPixelConfig: // fall through
        case kAlpha_8_as_Alpha_GrPixelConfig: // fall through
        case kAlpha_8_as_Red_GrPixelConfig:
            ct = kAlpha_8_SkColorType;
            break;
        case kGray_8_GrPixelConfig:
            ct = kGray_8_SkColorType;
            break;
        case kRGB_565_GrPixelConfig:
            ct = kRGB_565_SkColorType;
            break;
        case kRGBA_4444_GrPixelConfig:
            ct = kARGB_4444_SkColorType;
            break;
        case kRGBA_8888_GrPixelConfig:
            ct = kRGBA_8888_SkColorType;
            break;
        case kBGRA_8888_GrPixelConfig:
            ct = kBGRA_8888_SkColorType;
            break;
        case kSRGBA_8888_GrPixelConfig:
            ct = kRGBA_8888_SkColorType;
            break;
        case kSBGRA_8888_GrPixelConfig:
            ct = kBGRA_8888_SkColorType;
            break;
        case kRGBA_half_GrPixelConfig:
            ct = kRGBA_F16_SkColorType;
            break;
        default:
            return false;
    }
    if (ctOut) {
        *ctOut = ct;
    }
    return true;
}

GrPixelConfig GrRenderableConfigForColorSpace(const SkColorSpace* colorSpace) {
    if (!colorSpace) {
        return kRGBA_8888_GrPixelConfig;
    } else if (colorSpace->gammaIsLinear()) {
        return kRGBA_half_GrPixelConfig;
    } else if (colorSpace->gammaCloseToSRGB()) {
        return kSRGBA_8888_GrPixelConfig;
    } else {
        SkDEBUGFAIL("No renderable config exists for color space with strange gamma");
        return kUnknown_GrPixelConfig;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////

static inline bool blend_requires_shader(const SkBlendMode mode) {
    return SkBlendMode::kDst != mode;
}

static inline bool skpaint_to_grpaint_impl(GrContext* context,
                                           const GrColorSpaceInfo& colorSpaceInfo,
                                           const SkPaint& skPaint,
                                           const SkMatrix& viewM,
                                           std::unique_ptr<GrFragmentProcessor>* shaderProcessor,
                                           SkBlendMode* primColorMode,
                                           GrPaint* grPaint) {
    grPaint->setAllowSRGBInputs(colorSpaceInfo.isGammaCorrect());

    // Convert SkPaint color to 4f format, including optional linearizing and gamut conversion.
    GrColor4f origColor = SkColorToUnpremulGrColor4f(skPaint.getColor(), colorSpaceInfo);

    // Setup the initial color considering the shader, the SkPaint color, and the presence or not
    // of per-vertex colors.
    std::unique_ptr<GrFragmentProcessor> shaderFP;
    if (!primColorMode || blend_requires_shader(*primColorMode)) {
        if (shaderProcessor) {
            shaderFP = std::move(*shaderProcessor);
        } else if (const auto* shader = as_SB(skPaint.getShader())) {
            shaderFP = shader->asFragmentProcessor(SkShaderBase::AsFPArgs(
                    context, &viewM, nullptr, skPaint.getFilterQuality(), &colorSpaceInfo));
        }
    }

    // Set this in below cases if the output of the shader/paint-color/paint-alpha/primXfermode is
    // a known constant value. In that case we can simply apply a color filter during this
    // conversion without converting the color filter to a GrFragmentProcessor.
    bool applyColorFilterToPaintColor = false;
    if (shaderFP) {
        if (primColorMode) {
            // There is a blend between the primitive color and the shader color. The shader sees
            // the opaque paint color. The shader's output is blended using the provided mode by
            // the primitive color. The blended color is then modulated by the paint's alpha.

            // The geometry processor will insert the primitive color to start the color chain, so
            // the GrPaint color will be ignored.

            GrColor4f shaderInput = origColor.opaque();
            shaderFP = GrFragmentProcessor::OverrideInput(std::move(shaderFP), shaderInput);
            shaderFP = GrXfermodeFragmentProcessor::MakeFromSrcProcessor(std::move(shaderFP),
                                                                         *primColorMode);

            // The above may return null if compose results in a pass through of the prim color.
            if (shaderFP) {
                grPaint->addColorFragmentProcessor(std::move(shaderFP));
            }

            // We can ignore origColor here - alpha is unchanged by gamma
            GrColor paintAlpha = SkColorAlphaToGrColor(skPaint.getColor());
            if (GrColor_WHITE != paintAlpha) {
                // No gamut conversion - paintAlpha is a (linear) alpha value, splatted to all
                // color channels. It's value should be treated as the same in ANY color space.
                grPaint->addColorFragmentProcessor(GrConstColorProcessor::Make(
                    GrColor4f::FromGrColor(paintAlpha),
                    GrConstColorProcessor::InputMode::kModulateRGBA));
            }
        } else {
            // The shader's FP sees the paint unpremul color
            grPaint->setColor4f(origColor);
            grPaint->addColorFragmentProcessor(std::move(shaderFP));
        }
    } else {
        if (primColorMode) {
            // There is a blend between the primitive color and the paint color. The blend considers
            // the opaque paint color. The paint's alpha is applied to the post-blended color.
            auto processor = GrConstColorProcessor::Make(origColor.opaque(),
                                                         GrConstColorProcessor::InputMode::kIgnore);
            processor = GrXfermodeFragmentProcessor::MakeFromSrcProcessor(std::move(processor),
                                                                          *primColorMode);
            if (processor) {
                grPaint->addColorFragmentProcessor(std::move(processor));
            }

            grPaint->setColor4f(origColor.opaque());

            // We can ignore origColor here - alpha is unchanged by gamma
            GrColor paintAlpha = SkColorAlphaToGrColor(skPaint.getColor());
            if (GrColor_WHITE != paintAlpha) {
                // No gamut conversion - paintAlpha is a (linear) alpha value, splatted to all
                // color channels. It's value should be treated as the same in ANY color space.
                grPaint->addColorFragmentProcessor(GrConstColorProcessor::Make(
                    GrColor4f::FromGrColor(paintAlpha),
                    GrConstColorProcessor::InputMode::kModulateRGBA));
            }
        } else {
            // No shader, no primitive color.
            grPaint->setColor4f(origColor.premul());
            applyColorFilterToPaintColor = true;
        }
    }

    SkColorFilter* colorFilter = skPaint.getColorFilter();
    if (colorFilter) {
        if (applyColorFilterToPaintColor) {
            // If we're in legacy mode, we *must* avoid using the 4f version of the color filter,
            // because that will combine with the linearized version of the stored color.
            if (colorSpaceInfo.isGammaCorrect()) {
                grPaint->setColor4f(GrColor4f::FromSkColor4f(
                    colorFilter->filterColor4f(origColor.toSkColor4f())).premul());
            } else {
                grPaint->setColor4f(SkColorToPremulGrColor4fLegacy(
                        colorFilter->filterColor(skPaint.getColor())));
            }
        } else {
            auto cfFP = colorFilter->asFragmentProcessor(context, colorSpaceInfo);
            if (cfFP) {
                grPaint->addColorFragmentProcessor(std::move(cfFP));
            } else {
                return false;
            }
        }
    }

    SkMaskFilter* maskFilter = skPaint.getMaskFilter();
    if (maskFilter) {
        GrFragmentProcessor* mfFP;
        if (maskFilter->asFragmentProcessor(&mfFP)) {
            grPaint->addCoverageFragmentProcessor(std::unique_ptr<GrFragmentProcessor>(mfFP));
        }
    }

    // When the xfermode is null on the SkPaint (meaning kSrcOver) we need the XPFactory field on
    // the GrPaint to also be null (also kSrcOver).
    SkASSERT(!grPaint->getXPFactory());
    if (!skPaint.isSrcOver()) {
        grPaint->setXPFactory(SkBlendMode_AsXPFactory(skPaint.getBlendMode()));
    }

#ifndef SK_IGNORE_GPU_DITHER
    // Conservative default, in case GrPixelConfigToColorType() fails.
    SkColorType ct = SkColorType::kRGB_565_SkColorType;
    GrPixelConfigToColorType(colorSpaceInfo.config(), &ct);
    if (SkPaintPriv::ShouldDither(skPaint, ct) && grPaint->numColorFragmentProcessors() > 0 &&
        !colorSpaceInfo.isGammaCorrect()) {
        auto ditherFP = GrDitherEffect::Make(colorSpaceInfo.config());
        if (ditherFP) {
            grPaint->addColorFragmentProcessor(std::move(ditherFP));
        }
    }
#endif
    return true;
}

bool SkPaintToGrPaint(GrContext* context, const GrColorSpaceInfo& colorSpaceInfo,
                      const SkPaint& skPaint, const SkMatrix& viewM, GrPaint* grPaint) {
    return skpaint_to_grpaint_impl(context, colorSpaceInfo, skPaint, viewM, nullptr, nullptr,
                                   grPaint);
}

/** Replaces the SkShader (if any) on skPaint with the passed in GrFragmentProcessor. */
bool SkPaintToGrPaintReplaceShader(GrContext* context,
                                   const GrColorSpaceInfo& colorSpaceInfo,
                                   const SkPaint& skPaint,
                                   std::unique_ptr<GrFragmentProcessor> shaderFP,
                                   GrPaint* grPaint) {
    if (!shaderFP) {
        return false;
    }
    return skpaint_to_grpaint_impl(context, colorSpaceInfo, skPaint, SkMatrix::I(), &shaderFP,
                                   nullptr, grPaint);
}

/** Ignores the SkShader (if any) on skPaint. */
bool SkPaintToGrPaintNoShader(GrContext* context,
                              const GrColorSpaceInfo& colorSpaceInfo,
                              const SkPaint& skPaint,
                              GrPaint* grPaint) {
    // Use a ptr to a nullptr to to indicate that the SkShader is ignored and not replaced.
    static std::unique_ptr<GrFragmentProcessor> kNullShaderFP(nullptr);
    static std::unique_ptr<GrFragmentProcessor>* kIgnoreShader = &kNullShaderFP;
    return skpaint_to_grpaint_impl(context, colorSpaceInfo, skPaint, SkMatrix::I(), kIgnoreShader,
                                   nullptr, grPaint);
}

/** Blends the SkPaint's shader (or color if no shader) with a per-primitive color which must
be setup as a vertex attribute using the specified SkBlendMode. */
bool SkPaintToGrPaintWithXfermode(GrContext* context,
                                  const GrColorSpaceInfo& colorSpaceInfo,
                                  const SkPaint& skPaint,
                                  const SkMatrix& viewM,
                                  SkBlendMode primColorMode,
                                  GrPaint* grPaint) {
    return skpaint_to_grpaint_impl(context, colorSpaceInfo, skPaint, viewM, nullptr, &primColorMode,
                                   grPaint);
}

bool SkPaintToGrPaintWithTexture(GrContext* context,
                                 const GrColorSpaceInfo& colorSpaceInfo,
                                 const SkPaint& paint,
                                 const SkMatrix& viewM,
                                 std::unique_ptr<GrFragmentProcessor> fp,
                                 bool textureIsAlphaOnly,
                                 GrPaint* grPaint) {
    std::unique_ptr<GrFragmentProcessor> shaderFP;
    if (textureIsAlphaOnly) {
        if (const auto* shader = as_SB(paint.getShader())) {
            shaderFP = shader->asFragmentProcessor(SkShaderBase::AsFPArgs(
                    context, &viewM, nullptr, paint.getFilterQuality(), &colorSpaceInfo));
            if (!shaderFP) {
                return false;
            }
            std::unique_ptr<GrFragmentProcessor> fpSeries[] = { std::move(shaderFP), std::move(fp) };
            shaderFP = GrFragmentProcessor::RunInSeries(fpSeries, 2);
        } else {
            shaderFP = GrFragmentProcessor::MakeInputPremulAndMulByOutput(std::move(fp));
        }
    } else {
        shaderFP = GrFragmentProcessor::MulOutputByInputAlpha(std::move(fp));
    }

    return SkPaintToGrPaintReplaceShader(context, colorSpaceInfo, paint, std::move(shaderFP),
                                         grPaint);
}


////////////////////////////////////////////////////////////////////////////////////////////////

GrSamplerState::Filter GrSkFilterQualityToGrFilterMode(SkFilterQuality paintFilterQuality,
                                                       const SkMatrix& viewM,
                                                       const SkMatrix& localM,
                                                       bool* doBicubic) {
    *doBicubic = false;
    GrSamplerState::Filter textureFilterMode;
    switch (paintFilterQuality) {
        case kNone_SkFilterQuality:
            textureFilterMode = GrSamplerState::Filter::kNearest;
            break;
        case kLow_SkFilterQuality:
            textureFilterMode = GrSamplerState::Filter::kBilerp;
            break;
        case kMedium_SkFilterQuality: {
            SkMatrix matrix;
            matrix.setConcat(viewM, localM);
            if (matrix.getMinScale() < SK_Scalar1) {
                textureFilterMode = GrSamplerState::Filter::kMipMap;
            } else {
                // Don't trigger MIP level generation unnecessarily.
                textureFilterMode = GrSamplerState::Filter::kBilerp;
            }
            break;
        }
        case kHigh_SkFilterQuality: {
            SkMatrix matrix;
            matrix.setConcat(viewM, localM);
            *doBicubic = GrBicubicEffect::ShouldUseBicubic(matrix, &textureFilterMode);
            break;
        }
        default:
            // Should be unreachable.  If not, fall back to mipmaps.
            textureFilterMode = GrSamplerState::Filter::kMipMap;
            break;

    }
    return textureFilterMode;
}
