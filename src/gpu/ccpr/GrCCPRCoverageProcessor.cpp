/*
 * Copyright 2017 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "GrCCPRCoverageProcessor.h"

#include "SkMakeUnique.h"
#include "ccpr/GrCCPRCubicShader.h"
#include "ccpr/GrCCPRQuadraticShader.h"
#include "ccpr/GrCCPRTriangleShader.h"
#include "glsl/GrGLSLFragmentShaderBuilder.h"

static GrVertexAttribType instance_array_format(GrCCPRCoverageProcessor::RenderPass renderPass) {
    switch (renderPass) {
        case GrCCPRCoverageProcessor::RenderPass::kTriangleHulls:
        case GrCCPRCoverageProcessor::RenderPass::kTriangleEdges:
        case GrCCPRCoverageProcessor::RenderPass::kTriangleCorners:
            return kInt4_GrVertexAttribType;
        case GrCCPRCoverageProcessor::RenderPass::kQuadraticHulls:
        case GrCCPRCoverageProcessor::RenderPass::kQuadraticCorners:
        case GrCCPRCoverageProcessor::RenderPass::kSerpentineHulls:
        case GrCCPRCoverageProcessor::RenderPass::kLoopHulls:
        case GrCCPRCoverageProcessor::RenderPass::kSerpentineCorners:
        case GrCCPRCoverageProcessor::RenderPass::kLoopCorners:
            return kInt2_GrVertexAttribType;
    }
    SK_ABORT("Unexpected GrCCPRCoverageProcessor::RenderPass.");
    return kInt4_GrVertexAttribType;
}

GrCCPRCoverageProcessor::GrCCPRCoverageProcessor(RenderPass renderPass, GrBuffer* pointsBuffer)
        : INHERITED(kGrCCPRCoverageProcessor_ClassID)
        , fRenderPass(renderPass)
        , fInstanceAttrib(this->addInstanceAttrib("instance", instance_array_format(fRenderPass))) {
    fPointsBufferAccess.reset(kRG_float_GrPixelConfig, pointsBuffer, kVertex_GrShaderFlag);
    this->addBufferAccess(&fPointsBufferAccess);

    this->setWillUseGeoShader();
}

void GrCCPRCoverageProcessor::Shader::emitVaryings(GrGLSLVaryingHandler* varyingHandler,
                                                   SkString* code, const char* position,
                                                   const char* coverage, const char* wind) {
    WindHandling windHandling = this->onEmitVaryings(varyingHandler, code, position, coverage,
                                                     wind);
    if (WindHandling::kNotHandled == windHandling) {
        varyingHandler->addFlatVarying("wind", &fWind);
        code->appendf("%s = %s;", fWind.gsOut(), wind);
    }
}

void GrCCPRCoverageProcessor::Shader::emitFragmentCode(const GrCCPRCoverageProcessor& proc,
                                                       GrGLSLPPFragmentBuilder* f,
                                                       const char* skOutputColor,
                                                       const char* skOutputCoverage) const {
    f->codeAppendf("half coverage = 0;");
    this->onEmitFragmentCode(f, "coverage");
    if (fWind.fsIn()) {
        f->codeAppendf("%s.a = coverage * %s;", skOutputColor, fWind.fsIn());
    } else {
        f->codeAppendf("%s.a = coverage;", skOutputColor);
    }
    f->codeAppendf("%s = half4(1);", skOutputCoverage);
#ifdef SK_DEBUG
    if (proc.debugVisualizationsEnabled()) {
        f->codeAppendf("%s = half4(-%s.a, %s.a, 0, 1);",
                       skOutputColor, skOutputColor, skOutputColor);
    }
#endif
}

void GrCCPRCoverageProcessor::Shader::EmitEdgeDistanceEquation(GrGLSLShaderBuilder* s,
                                                               const char* leftPt,
                                                               const char* rightPt,
                                                               const char* outputDistanceEquation) {
    s->codeAppendf("float2 n = float2(%s.y - %s.y, %s.x - %s.x);",
                   rightPt, leftPt, leftPt, rightPt);
    s->codeAppend ("float nwidth = (abs(n.x) + abs(n.y)) * (bloat * 2);");
    // When nwidth=0, wind must also be 0 (and coverage * wind = 0). So it doesn't matter what we
    // come up with here as long as it isn't NaN or Inf.
    s->codeAppend ("n /= (0 != nwidth) ? nwidth : 1;");
    s->codeAppendf("%s = float3(-n, dot(n, %s) - .5);", outputDistanceEquation, leftPt);
}

int GrCCPRCoverageProcessor::Shader::DefineSoftSampleLocations(GrGLSLPPFragmentBuilder* f,
                                                               const char* samplesName) {
    // Standard DX11 sample locations.
#if defined(SK_BUILD_FOR_ANDROID) || defined(SK_BUILD_FOR_IOS)
    f->defineConstant("float2[8]", samplesName, "float2[8]("
        "float2(+1, -3)/16, float2(-1, +3)/16, float2(+5, +1)/16, float2(-3, -5)/16, "
        "float2(-5, +5)/16, float2(-7, -1)/16, float2(+3, +7)/16, float2(+7, -7)/16."
    ")");
    return 8;
#else
    f->defineConstant("float2[16]", samplesName, "float2[16]("
        "float2(+1, +1)/16, float2(-1, -3)/16, float2(-3, +2)/16, float2(+4, -1)/16, "
        "float2(-5, -2)/16, float2(+2, +5)/16, float2(+5, +3)/16, float2(+3, -5)/16, "
        "float2(-2, +6)/16, float2( 0, -7)/16, float2(-4, -6)/16, float2(-6, +4)/16, "
        "float2(-8,  0)/16, float2(+7, -4)/16, float2(+6, +7)/16, float2(-7, -8)/16."
    ")");
    return 16;
#endif
}

void GrCCPRCoverageProcessor::getGLSLProcessorKey(const GrShaderCaps&,
                                                  GrProcessorKeyBuilder* b) const {
    b->add32((int)fRenderPass);
}

GrGLSLPrimitiveProcessor* GrCCPRCoverageProcessor::createGLSLInstance(const GrShaderCaps&) const {
    std::unique_ptr<Shader> shader;
    switch (fRenderPass) {
        using CubicType = GrCCPRCubicShader::CubicType;
        case RenderPass::kTriangleHulls:
            shader = skstd::make_unique<GrCCPRTriangleHullShader>();
            break;
        case RenderPass::kTriangleEdges:
            shader = skstd::make_unique<GrCCPRTriangleEdgeShader>();
            break;
        case RenderPass::kTriangleCorners:
            shader = skstd::make_unique<GrCCPRTriangleCornerShader>();
            break;
        case RenderPass::kQuadraticHulls:
            shader = skstd::make_unique<GrCCPRQuadraticHullShader>();
            break;
        case RenderPass::kQuadraticCorners:
            shader = skstd::make_unique<GrCCPRQuadraticCornerShader>();
            break;
        case RenderPass::kSerpentineHulls:
            shader = skstd::make_unique<GrCCPRCubicHullShader>(CubicType::kSerpentine);
            break;
        case RenderPass::kLoopHulls:
            shader = skstd::make_unique<GrCCPRCubicHullShader>(CubicType::kLoop);
            break;
        case RenderPass::kSerpentineCorners:
            shader = skstd::make_unique<GrCCPRCubicCornerShader>(CubicType::kSerpentine);
            break;
        case RenderPass::kLoopCorners:
            shader = skstd::make_unique<GrCCPRCubicCornerShader>(CubicType::kLoop);
            break;
    }
    return CreateGSImpl(std::move(shader));
}

const char* GrCCPRCoverageProcessor::GetRenderPassName(RenderPass renderPass) {
    switch (renderPass) {
        case RenderPass::kTriangleHulls:
            return "RenderPass::kTriangleHulls";
        case RenderPass::kTriangleEdges:
            return "RenderPass::kTriangleEdges";
        case RenderPass::kTriangleCorners:
            return "RenderPass::kTriangleCorners";
        case RenderPass::kQuadraticHulls:
            return "RenderPass::kQuadraticHulls";
        case RenderPass::kQuadraticCorners:
            return "RenderPass::kQuadraticCorners";
        case RenderPass::kSerpentineHulls:
            return "RenderPass::kSerpentineHulls";
        case RenderPass::kLoopHulls:
            return "RenderPass::kLoopHulls";
        case RenderPass::kSerpentineCorners:
            return "RenderPass::kSerpentineCorners";
        case RenderPass::kLoopCorners:
            return "RenderPass::kLoopCorners";
    }
    SK_ABORT("Unexpected GrCCPRCoverageProcessor::RenderPass.");
    return nullptr;
}
