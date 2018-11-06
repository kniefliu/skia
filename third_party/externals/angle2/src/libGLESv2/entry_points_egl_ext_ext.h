// entry_points_egl_ext_ext.h : Defines the EGL extension extension entry points.

#ifndef LIBGLESV2_ENTRYPOINTSEGLEXTEXT_H_
#define LIBGLESV2_ENTRYPOINTSEGLEXTEXT_H_

#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <export.h>

#include <platform/platform.h>

namespace egl
{

ANGLE_EXPORT EGLBoolean EGLAPIENTRY RegisterPlatformMethodsANGLE(EGLDisplay dpy, angle::PlatformMethods* platformMethods);

}  // namespace egl

#endif // LIBGLESV2_ENTRYPOINTSEGLEXTEXT_H_
