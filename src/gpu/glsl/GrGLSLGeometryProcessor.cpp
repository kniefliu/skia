/*
 * Copyright 2014 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "GrGLSLGeometryProcessor.h"

#include "GrCoordTransform.h"
#include "glsl/GrGLSLFragmentShaderBuilder.h"
#include "glsl/GrGLSLUniformHandler.h"
#include "glsl/GrGLSLVarying.h"
#include "glsl/GrGLSLVertexGeoBuilder.h"

void GrGLSLGeometryProcessor::emitCode(EmitArgs& args) {
    GrGPArgs gpArgs;
    this->onEmitCode(args, &gpArgs);
    SkASSERT(kFloat2_GrSLType == gpArgs.fPositionVar.getType() ||
             kFloat3_GrSLType == gpArgs.fPositionVar.getType());

    GrGLSLVertexBuilder* vBuilder = args.fVertBuilder;
    if (!args.fGP.willUseGeoShader()) {
        // Emit the vertex position to the hardware in the normalized window coordinates it expects.
        vBuilder->emitNormalizedSkPosition(gpArgs.fPositionVar.c_str(), args.fRTAdjustName,
                                           gpArgs.fPositionVar.getType());
    } else {
        // Since we have a geometry shader, leave the vertex position in Skia device space for now.
        // The geometry Shader will operate in device space, and then convert the final positions to
        // normalized hardware window coordinates under the hood, once everything else has finished.
        vBuilder->codeAppendf("sk_Position = float4(%s", gpArgs.fPositionVar.c_str());
        if (kFloat2_GrSLType == gpArgs.fPositionVar.getType()) {
            vBuilder->codeAppend(", 0");
        }
        vBuilder->codeAppend(", 1);");
    }

    if (kFloat2_GrSLType == gpArgs.fPositionVar.getType()) {
        args.fVaryingHandler->setNoPerspective();
    }
}

void GrGLSLGeometryProcessor::emitTransforms(GrGLSLVertexBuilder* vb,
                                             GrGLSLVaryingHandler* varyingHandler,
                                             GrGLSLUniformHandler* uniformHandler,
                                             const GrShaderVar& posVar,
                                             const char* localCoords,
                                             const SkMatrix& localMatrix,
                                             FPCoordTransformHandler* handler) {
    int i = 0;
    while (const GrCoordTransform* coordTransform = handler->nextCoordTransform()) {
        SkString strUniName;
        strUniName.printf("CoordTransformMatrix_%d", i);
        GrSLType varyingType;

        uint32_t type = coordTransform->getMatrix().getType();
        type |= localMatrix.getType();

        varyingType = SkToBool(SkMatrix::kPerspective_Mask & type) ? kFloat3_GrSLType :
                                                                     kFloat2_GrSLType;
        const char* uniName;


        fInstalledTransforms.push_back().fHandle = uniformHandler->addUniform(kVertex_GrShaderFlag,
                                                                              kFloat3x3_GrSLType,
                                                                              strUniName.c_str(),
                                                                              &uniName).toIndex();
        SkString strVaryingName;
        strVaryingName.printf("TransformedCoords_%d", i);

        GrGLSLVertToFrag v(varyingType);
        varyingHandler->addVarying(strVaryingName.c_str(), &v);

        SkASSERT(kFloat2_GrSLType == varyingType || kFloat3_GrSLType == varyingType);
        handler->specifyCoordsForCurrCoordTransform(SkString(v.fsIn()), varyingType);

        if (kFloat2_GrSLType == varyingType) {
            vb->codeAppendf("%s = (%s * float3(%s, 1)).xy;", v.vsOut(), uniName, localCoords);
        } else {
            vb->codeAppendf("%s = %s * float3(%s, 1);", v.vsOut(), uniName, localCoords);
        }
        ++i;
    }
}

void GrGLSLGeometryProcessor::setTransformDataHelper(const SkMatrix& localMatrix,
                                                     const GrGLSLProgramDataManager& pdman,
                                                     FPCoordTransformIter* transformIter) {
    int i = 0;
    while (const GrCoordTransform* coordTransform = transformIter->next()) {
        const SkMatrix& m = GetTransformMatrix(localMatrix, *coordTransform);
        if (!fInstalledTransforms[i].fCurrentValue.cheapEqualTo(m)) {
            pdman.setSkMatrix(fInstalledTransforms[i].fHandle.toIndex(), m);
            fInstalledTransforms[i].fCurrentValue = m;
        }
        ++i;
    }
    SkASSERT(i == fInstalledTransforms.count());
}

void GrGLSLGeometryProcessor::writeOutputPosition(GrGLSLVertexBuilder* vertBuilder,
                                                  GrGPArgs* gpArgs,
                                                  const char* posName) {
    gpArgs->fPositionVar.set(kFloat2_GrSLType, "pos2");
    vertBuilder->codeAppendf("float2 %s = %s;", gpArgs->fPositionVar.c_str(), posName);
}

void GrGLSLGeometryProcessor::writeOutputPosition(GrGLSLVertexBuilder* vertBuilder,
                                                  GrGLSLUniformHandler* uniformHandler,
                                                  GrGPArgs* gpArgs,
                                                  const char* posName,
                                                  const SkMatrix& mat,
                                                  UniformHandle* viewMatrixUniform) {
    if (mat.isIdentity()) {
        gpArgs->fPositionVar.set(kFloat2_GrSLType, "pos2");
        vertBuilder->codeAppendf("float2 %s = %s;", gpArgs->fPositionVar.c_str(), posName);
    } else {
        const char* viewMatrixName;
        *viewMatrixUniform = uniformHandler->addUniform(kVertex_GrShaderFlag,
                                                        kFloat3x3_GrSLType,
                                                        "uViewM",
                                                        &viewMatrixName);
        if (!mat.hasPerspective()) {
            gpArgs->fPositionVar.set(kFloat2_GrSLType, "pos2");
            vertBuilder->codeAppendf("float2 %s = (%s * float3(%s, 1)).xy;",
                                     gpArgs->fPositionVar.c_str(), viewMatrixName, posName);
        } else {
            gpArgs->fPositionVar.set(kFloat3_GrSLType, "pos3");
            vertBuilder->codeAppendf("float3 %s = %s * float3(%s, 1);",
                                     gpArgs->fPositionVar.c_str(), viewMatrixName, posName);
        }
    }
}
