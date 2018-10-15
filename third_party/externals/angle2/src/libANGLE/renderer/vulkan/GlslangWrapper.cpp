//
// Copyright (c) 2016 The ANGLE Project Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// GlslangWrapper: Wrapper for Vulkan's glslang compiler.
//

#include "libANGLE/renderer/vulkan/GlslangWrapper.h"

// glslang's version of ShaderLang.h, not to be confused with ANGLE's.
// Our function defs conflict with theirs, but we carefully manage our includes to prevent this.
#include <ShaderLang.h>

// Other glslang includes.
#include <StandAlone/ResourceLimits.h>
#include <SPIRV/GlslangToSpv.h>

#include <array>

#include "common/string_utils.h"

namespace rx
{

// static
GlslangWrapper *GlslangWrapper::mInstance = nullptr;

// static
GlslangWrapper *GlslangWrapper::GetReference()
{
    if (!mInstance)
    {
        mInstance = new GlslangWrapper();
    }

    mInstance->addRef();

    return mInstance;
}

// static
void GlslangWrapper::ReleaseReference()
{
    if (mInstance->getRefCount() == 1)
    {
        mInstance->release();
        mInstance = nullptr;
    }
    else
    {
        mInstance->release();
    }
}

GlslangWrapper::GlslangWrapper()
{
    int result = ShInitialize();
    ASSERT(result != 0);
}

GlslangWrapper::~GlslangWrapper()
{
    int result = ShFinalize();
    ASSERT(result != 0);
}

gl::LinkResult GlslangWrapper::linkProgram(const gl::Context *glContext,
                                           const gl::ProgramState &programState,
                                           std::vector<uint32_t> *vertexCodeOut,
                                           std::vector<uint32_t> *fragmentCodeOut)
{
    std::string vertexSource =
        programState.getAttachedVertexShader()->getTranslatedSource(glContext);
    std::string fragmentSource =
        programState.getAttachedFragmentShader()->getTranslatedSource(glContext);

    // Parse attribute locations and replace them in the vertex shader.
    // See corresponding code in OutputVulkanGLSL.cpp.
    // TODO(jmadill): Also do the same for ESSL 3 fragment outputs.
    for (const auto &attribute : programState.getAttributes())
    {
        if (!attribute.staticUse)
            continue;

        std::stringstream searchStringBuilder;
        searchStringBuilder << "@@ LOCATION-" << attribute.name << " @@";
        std::string searchString = searchStringBuilder.str();

        std::string locationString = Str(attribute.location);

        bool success = angle::ReplaceSubstring(&vertexSource, searchString, locationString);
        ASSERT(success);
    }

    // Bind the default uniforms for vertex and fragment shaders.
    // See corresponding code in OutputVulkanGLSL.cpp.
    std::stringstream searchStringBuilder;
    searchStringBuilder << "@@ DEFAULT-UNIFORMS-SET-BINDING @@";
    std::string searchString = searchStringBuilder.str();

    std::string vertexDefaultUniformsBinding   = "set = 0, binding = 0";
    std::string fragmentDefaultUniformsBinding = "set = 0, binding = 1";

    angle::ReplaceSubstring(&vertexSource, searchString, vertexDefaultUniformsBinding);
    angle::ReplaceSubstring(&fragmentSource, searchString, fragmentDefaultUniformsBinding);

    std::array<const char *, 2> strings = {{vertexSource.c_str(), fragmentSource.c_str()}};

    std::array<int, 2> lengths = {
        {static_cast<int>(vertexSource.length()), static_cast<int>(fragmentSource.length())}};

    // Enable SPIR-V and Vulkan rules when parsing GLSL
    EShMessages messages = static_cast<EShMessages>(EShMsgSpvRules | EShMsgVulkanRules);

    glslang::TShader vertexShader(EShLangVertex);
    vertexShader.setStringsWithLengths(&strings[0], &lengths[0], 1);
    vertexShader.setEntryPoint("main");
    bool vertexResult = vertexShader.parse(&glslang::DefaultTBuiltInResource, 450, ECoreProfile,
                                           false, false, messages);
    if (!vertexResult)
    {
        return gl::InternalError() << "Internal error parsing Vulkan vertex shader:\n"
                                   << vertexShader.getInfoLog() << "\n"
                                   << vertexShader.getInfoDebugLog() << "\n";
    }

    glslang::TShader fragmentShader(EShLangFragment);
    fragmentShader.setStringsWithLengths(&strings[1], &lengths[1], 1);
    fragmentShader.setEntryPoint("main");
    bool fragmentResult = fragmentShader.parse(&glslang::DefaultTBuiltInResource, 450, ECoreProfile,
                                               false, false, messages);
    if (!fragmentResult)
    {
        return gl::InternalError() << "Internal error parsing Vulkan fragment shader:\n"
                                   << fragmentShader.getInfoLog() << "\n"
                                   << fragmentShader.getInfoDebugLog() << "\n";
    }

    glslang::TProgram program;
    program.addShader(&vertexShader);
    program.addShader(&fragmentShader);
    bool linkResult = program.link(messages);
    if (!linkResult)
    {
        return gl::InternalError() << "Internal error linking Vulkan shaders:\n"
                                   << program.getInfoLog() << "\n";
    }

    glslang::TIntermediate *vertexStage   = program.getIntermediate(EShLangVertex);
    glslang::TIntermediate *fragmentStage = program.getIntermediate(EShLangFragment);
    glslang::GlslangToSpv(*vertexStage, *vertexCodeOut);
    glslang::GlslangToSpv(*fragmentStage, *fragmentCodeOut);

    return true;
}

}  // namespace rx
