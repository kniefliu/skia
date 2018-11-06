// entry_points_ext_ext.cpp : Implements the EGL extension extension entry points.

#include "libGLESv2/entry_points_egl_ext_ext.h"
#include "libGLESv2/global_state.h"

#include "libANGLE/Context.h"
#include "libANGLE/Display.h"
#include "libANGLE/Device.h"
#include "libANGLE/Surface.h"
#include "libANGLE/Stream.h"
#include "libANGLE/Thread.h"
#include "libANGLE/validationEGL.h"

#include "common/debug.h"

namespace egl
{

// 
EGLBoolean EGLAPIENTRY RegisterPlatformMethodsANGLE(EGLDisplay dpy, angle::PlatformMethods* platformMethods)
{
    Display *display = static_cast<Display*>(dpy);

	*ANGLEPlatformCurrent() = *platformMethods;

	return EGL_TRUE;
}

}  // namespace egl
