/*
 * Copyright 2011 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrGLSL_DEFINED
#define GrGLSL_DEFINED

#include "GrTypesPriv.h"
#include "SkString.h"

class GrShaderCaps;

// Limited set of GLSL versions we build shaders for. Caller should round
// down the GLSL version to one of these enums.
enum GrGLSLGeneration {
    /**
     * Desktop GLSL 1.10 and ES2 shading language (based on desktop GLSL 1.20)
     */
    k110_GrGLSLGeneration,
    /**
     * Desktop GLSL 1.30
     */
    k130_GrGLSLGeneration,
    /**
     * Desktop GLSL 1.40
     */
    k140_GrGLSLGeneration,
    /**
     * Desktop GLSL 1.50
     */
    k150_GrGLSLGeneration,
    /**
     * Desktop GLSL 3.30, and ES GLSL 3.00
     */
    k330_GrGLSLGeneration,
    /**
     * Desktop GLSL 4.00
     */
    k400_GrGLSLGeneration,
    /**
     * Desktop GLSL 4.20
     */
    k420_GrGLSLGeneration,
    /**
     * ES GLSL 3.10 only TODO Make GLSLCap objects to make this more granular
     */
    k310es_GrGLSLGeneration,
    /**
     * ES GLSL 3.20
     */
    k320es_GrGLSLGeneration,
};

bool GrGLSLSupportsNamedFragmentShaderOutputs(GrGLSLGeneration);

/**
 * Adds a line of GLSL code to declare the default precision for float types.
 */
void GrGLSLAppendDefaultFloatPrecisionDeclaration(GrSLPrecision,
                                                  const GrShaderCaps&,
                                                  SkString* out);

/**
 * Converts a GrSLPrecision to its corresponding GLSL precision qualifier.
 */
static inline const char* GrGLSLPrecisionString(GrSLPrecision p) {
    switch (p) {
        case kLow_GrSLPrecision:
            return "lowp";
        case kMedium_GrSLPrecision:
            return "mediump";
        case kHigh_GrSLPrecision:
            return "highp";
        case kDefault_GrSLPrecision:
            return "";
        default:
            SK_ABORT("Unexpected precision type.");
            return "";
    }
}

/**
 * Converts a GrSLType to a string containing the name of the equivalent GLSL type.
 */
static inline const char* GrGLSLTypeString(GrSLType t) {
    switch (t) {
        case kVoid_GrSLType:
            return "void";
        case kFloat_GrSLType:
            return "float";
        case kVec2f_GrSLType:
            return "float2";
        case kVec3f_GrSLType:
            return "float3";
        case kVec4f_GrSLType:
            return "float4";
        case kVec2i_GrSLType:
            return "int2";
        case kVec3i_GrSLType:
            return "int3";
        case kVec4i_GrSLType:
            return "int4";
        case kMat22f_GrSLType:
            return "float2x2";
        case kMat33f_GrSLType:
            return "float3x3";
        case kMat44f_GrSLType:
            return "float4x4";
        case kTexture2DSampler_GrSLType:
            return "sampler2D";
        case kITexture2DSampler_GrSLType:
            return "isampler2D";
        case kTextureExternalSampler_GrSLType:
            return "samplerExternalOES";
        case kTexture2DRectSampler_GrSLType:
            return "sampler2DRect";
        case kBufferSampler_GrSLType:
            return "samplerBuffer";
        case kBool_GrSLType:
            return "bool";
        case kInt_GrSLType:
            return "int";
        case kUint_GrSLType:
            return "uint";
        case kTexture2D_GrSLType:
            return "texture2D";
        case kSampler_GrSLType:
            return "sampler";
        case kImageStorage2D_GrSLType:
            return "image2D";
        case kIImageStorage2D_GrSLType:
            return "iimage2D";
    }
    SK_ABORT("Unknown shader var type.");
    return ""; // suppress warning
}

#endif
