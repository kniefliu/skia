
/*
 * Copyright 2012 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */
#ifndef GLContext_angle_DEFINED
#define GLContext_angle_DEFINED

#include "SkImageInfo.h"
#include "SkSurface.h"
#include "gl/GrGLInterface.h"

namespace sk_gpu_angle {

/**
 * Creates a GrGLInterface for the current ANGLE GLES Context. Here current means bound in ANGLE's
 * implementation of EGL.
 */
SK_API const GrGLInterface* CreateANGLEGLInterface();

SK_API sk_sp<SkSurface> makeGpuBackedSurface(int fSampleCount, int fStencilBits, int fColorBits,
	int width, int height, const SkImageInfo& info, SkSurfaceProps  fSurfaceProps,
	const GrGLInterface* interface,
	GrContext* grContext, GrSurfaceOrigin origin = kTopLeft_GrSurfaceOrigin);

}  // namespace 
#endif
