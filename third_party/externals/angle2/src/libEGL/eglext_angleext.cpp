#include "EGL/eglext_angleext.h"
#include "libGLESv2/entry_points_egl_ext_ext.h"

extern "C"
{

bool registerPlatformMethods(EGLDisplay dpy, angle::PlatformMethods* platformMethods)
{
    return egl::RegisterPlatformMethodsANGLE(dpy, platformMethods);
}

}  // extern "C"
