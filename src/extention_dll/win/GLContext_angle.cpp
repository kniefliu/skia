/*
 * Copyright 2012 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "GLContext_angle.h"

#include <EGL/egl.h>
#include <EGL/eglext.h>

#include "gl/GrGLDefines.h"
#include "gl/GrGLUtil.h"

#include "gl/GrGLInterface.h"
#include "gl/GrGLAssembleInterface.h"
#include "../ports/SkOSLibrary.h"

#include <EGL/egl.h>

#include "GrBackendSurface.h"
#include "GrContext.h"
#include "gl/GrGLInterface.h"
#include "gl/GrGLUtil.h"
#include "SkGr.h"
#include "SkSurface.h"

#define EGL_PLATFORM_ANGLE_ANGLE                0x3202
#define EGL_PLATFORM_ANGLE_TYPE_ANGLE           0x3203
#define EGL_PLATFORM_ANGLE_TYPE_D3D9_ANGLE      0x3207
#define EGL_PLATFORM_ANGLE_TYPE_D3D11_ANGLE     0x3208
#define EGL_PLATFORM_ANGLE_TYPE_OPENGL_ANGLE    0x320D

namespace {
	struct Libs {
		void* fGLLib;
		void* fEGLLib;
	};


	static GrGLFuncPtr angle_get_gl_proc(void* ctx, const char name[]) {
		const Libs* libs = reinterpret_cast<const Libs*>(ctx);
		GrGLFuncPtr proc = (GrGLFuncPtr)GetProcedureAddress(libs->fGLLib, name);
		if (proc) {
			return proc;
		}
		proc = (GrGLFuncPtr)GetProcedureAddress(libs->fEGLLib, name);
		if (proc) {
			return proc;
		}
		return eglGetProcAddress(name);
	}
}

namespace sk_gpu_angle {
const GrGLInterface* CreateANGLEGLInterface() {
    static Libs gLibs = { nullptr, nullptr };

    if (nullptr == gLibs.fGLLib) {
        // We load the ANGLE library and never let it go
#if defined _WIN32
        gLibs.fGLLib = DynamicLoadLibrary("libGLESv2.dll");
        gLibs.fEGLLib = DynamicLoadLibrary("libEGL.dll");
#elif defined SK_BUILD_FOR_MAC
        gLibs.fGLLib = DynamicLoadLibrary("libGLESv2.dylib");
        gLibs.fEGLLib = DynamicLoadLibrary("libEGL.dylib");
#else
        gLibs.fGLLib = DynamicLoadLibrary("libGLESv2.so");
        gLibs.fEGLLib = DynamicLoadLibrary("libEGL.so");
#endif
    }

    if (nullptr == gLibs.fGLLib || nullptr == gLibs.fEGLLib) {
        // We can't setup the interface correctly w/o the so
        return nullptr;
    }

    return GrGLAssembleGLESInterface(&gLibs, angle_get_gl_proc);
}



sk_sp<SkSurface> makeGpuBackedSurface(int fSampleCount, int fStencilBits, int fColorBits,
	int width, int height, const SkImageInfo& info, SkSurfaceProps  fSurfaceProps,
	const GrGLInterface* interface,
	GrContext* grContext, GrSurfaceOrigin origin/* = kTopLeft_GrSurfaceOrigin*/)

{

	if (0 == width || 0 == height) {
		return nullptr;
	}

	// TODO: Query the actual framebuffer for sRGB capable. However, to
	// preserve old (fake-linear) behavior, we don't do this. Instead, rely
	// on the flag (currently driven via 'C' mode in SampleApp).
	//
	// Also, we may not have real sRGB support (ANGLE, in particular), so check for
	// that, and fall back to L32:
	//
	// ... and, if we're using a 10-bit/channel FB0, it doesn't do sRGB conversion on write,
	// so pretend that it's non-sRGB 8888:
	GrPixelConfig config = grContext->caps()->srgbSupport() &&
		info.colorSpace() &&
		(fColorBits != 30)
		? kSRGBA_8888_GrPixelConfig : kRGBA_8888_GrPixelConfig;
	GrGLFramebufferInfo fbInfo;
	GrGLint buffer;
	GR_GL_GetIntegerv(interface, GR_GL_FRAMEBUFFER_BINDING, &buffer);
	fbInfo.fFBOID = buffer;

	GrBackendRenderTarget backendRT(width,
		height,
		fSampleCount,
		fStencilBits,
		config,
		fbInfo);


	sk_sp<SkColorSpace> colorSpace =
		grContext->caps()->srgbSupport() && info.colorSpace()
		? SkColorSpace::MakeSRGB() : nullptr;
	return SkSurface::MakeFromBackendRenderTarget(grContext, backendRT, origin,
		colorSpace, &fSurfaceProps);
}

}  //
