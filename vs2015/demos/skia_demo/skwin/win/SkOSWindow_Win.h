#ifndef SkOSWindow_Win_DEFINED
#define SkOSWindow_Win_DEFINED

#if SK_SUPPORT_GPU
#if SK_ANGLE
#include "EGL/egl.h"
#endif
#include "GrContextOptions.h"

struct GrGLInterface;
class GrContext;
class GrRenderTarget;
#endif

class SkOSWindow : public SkWindow {
public:
    SkOSWindow(void* hwnd);
    virtual ~SkOSWindow();

	void setDeviceType(DeviceType type) override;
	DeviceType getDeviceType() override;

	void init() override;
	void unInit() override;

	void doPaint(void* ctx);

	sk_sp<SkSurface> makeSurface() override;

protected:
	void draw(SkCanvas* canvas) override;

	bool attach();
	void detach();
	void present();

	void setUpBackend();
	void tearDownBackend();

	void publishCanvas(SkCanvas* canvas);
	void onSizeChange();

private:
	void onDrawTitle(SkCanvas * canvas);
	void onSetTitle();

private:
#if SK_SUPPORT_GPU
	sk_sp<SkSurface> makeGPUSurface();

    bool attachGL();
    void detachGL();
    void presentGL();

#if SK_ANGLE
    bool attachANGLE();
    void detachANGLE();
    void presentANGLE();
#endif // SK_ANGLE
#endif // SK_SUPPORT_GPU

private:
	void*               fHWND;
#if SK_SUPPORT_GPU
	GrContext*              fCurContext;
	const GrGLInterface*    fCurIntf;
	sk_sp<SkSurface>        fGpuSurface;
	int fMSAASampleCount;
	bool fDeepColor;
	int fActualColorBits;
	void*               fHGLRC;

	GrSurfaceOrigin origin_;

#if SK_ANGLE
	EGLDisplay          fDisplay;
	EGLContext          fContext;
	EGLSurface          fSurface;
	EGLConfig           fConfig;
	sk_sp<const GrGLInterface> fANGLEInterface;
#endif // SK_ANGLE
#endif

	DeviceType fExpectedDeviceType;
	DeviceType fActualUsedDeviceType;

	bool need_get_origin_title_;
	std::string origin_title_;
	std::string current_title_;

private:
	typedef SkWindow INHERITED;
};

#endif
