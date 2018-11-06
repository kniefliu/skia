#ifndef __EGLEXT_ANGLEEXT_H__
#define __EGLEXT_ANGLEEXT_H__

#ifdef EGLEXT_ANGLEEXT_EXPORT
#define EGLEXT_ANGLEEXTAPI __declspec(dllexport)
#else
#define EGLEXT_ANGLEEXTAPI __declspec(dllimport)
#endif

#include <EGL/egl.h>
#include "platform/platform.h"

#if defined(__cplusplus)
extern "C" {
#endif

/**
 * register platform methods
 * currently used for log.
 */
EGLEXT_ANGLEEXTAPI bool registerPlatformMethods(EGLDisplay dpy, angle::PlatformMethods* platformMethods);

#if defined(__cplusplus)
}
#endif

#endif//__EGLEXT_ANGLEEXT_H__
