#include "..\SkWindow.h"

#include <WindowsX.h>
#include <Windows.h>

#include "SkEventTracer.h"

// start : ignore skia dll warnings
#pragma warning( push )  
#pragma warning( disable : 4251 )  

#if SK_SUPPORT_GPU
#else
class GrContext;
#endif

#include "SkTypes.h"
#include "SkCanvas.h"
#include "SkGraphics.h"

// end : ignore skia dll warnings
#pragma warning( pop )

#include "SkColorFilter.h"

#if SK_SUPPORT_GPU
#include "gl/GrGLInterface.h"
#include "GLContext_angle.h"
#include "GrRenderTarget.h"
#include "GrContextOptions.h"
#include "GrContext.h"
#include "GrContextOptions.h"
#include "SkWGL.h"
#if SK_ANGLE
#include "gl/GrGLAssembleInterface.h"
#include "gl/GrGLInterface.h"
#include "GLES2/gl2.h"
#include <EGL/egl.h>
#include <EGL/eglext.h>

#include <EGL/eglext_angleext.h>

#define GL_CALL(IFACE, X)                                 \
    SkASSERT(IFACE);                                      \
    do {                                                  \
        (IFACE)->fFunctions.f##X;                         \
    } while (false)

#endif
#endif

#include <sstream>


int g_LogFileRef = 0;
FILE * g_LogFile = nullptr;
EGLDisplay g_Display = EGL_NO_DISPLAY;
double g_FrequencyQuadPart;
int g_LogDepth = 0;
int g_LogPreDepth = -1;
char g_LogDepthBuf[32] = { 0 };

inline void LogMessage(const char* buf)
{
	if (g_LogDepth != g_LogPreDepth) {
		// 
		for (int i = 0; i < g_LogDepth; i++) {
			g_LogDepthBuf[i] = ' ';
		}
		g_LogDepthBuf[g_LogDepth] = '\0';
		g_LogPreDepth = g_LogDepth;
	}


	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);
	double time_msec = (double)time.QuadPart / g_FrequencyQuadPart;
	
	if (g_LogFile)
		fprintf(g_LogFile, "%6.3f : %s%s", time_msec, g_LogDepthBuf, buf);
}
inline void LogNewLine()
{
	if (g_LogFile)
		fputs("\n", g_LogFile);
}

class StartEndLog
{
public:
	StartEndLog(const std::string& logInfo) 
		: log_info_start_("Start " + logInfo + "\n")
		, log_info_end_("End " + logInfo + "\n")
	{
		LogMessage(log_info_start_.c_str());
		g_LogDepth++;
	}
	~StartEndLog()
	{
		g_LogDepth--;
		LogMessage(log_info_end_.c_str());
	}
	std::string log_info_start_;
	std::string log_info_end_;
};

bool IsGpuDeviceType(SkOSWindow::DeviceType devType) {
#if SK_SUPPORT_GPU
	switch (devType) {
	case SkOSWindow::kGPU_DeviceType:
#if SK_ANGLE
	case SkOSWindow::kANGLE_DeviceTypeD3D11:
	case SkOSWindow::kANGLE_DeviceTypeD3D9:
#endif // SK_ANGLE
		return true;
	default:
		return false;
	}
#endif // SK_SUPPORT_GPU
	return false;
}

void globalInit()
{
	if (!g_LogFile) {
		g_LogFile = fopen("skia_demo.txt", "wt");

		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		g_FrequencyQuadPart = (double)frequency.QuadPart;

		//EGLint majorVersion, minorVersion;
		//g_Display = eglGetDisplay(GetDC(GetDesktopWindow()));
		//if (!eglInitialize(g_Display, &majorVersion, &minorVersion)) {

		//}
	}
	g_LogFileRef++;

	
}
void globalUnInit()
{
	g_LogFileRef--;
	if (g_LogFileRef == 0) {
		if (g_LogFile)
			fclose(g_LogFile);
		g_LogFile = nullptr;
		//eglTerminate(g_Display);
	}
}

SkOSWindow::SkOSWindow(void* hWnd) 
{
	globalInit();

	need_get_origin_title_ = true;
	layered_ = false;
	fHWND = hWnd;

#if SK_SUPPORT_GPU
	fCurContext = nullptr;
	fCurIntf = nullptr;
	fMSAASampleCount = 0;
	fDeepColor = false;
	fActualColorBits = 0;
	fHGLRC = NULL;

#if SK_ANGLE
    fDisplay = EGL_NO_DISPLAY;
    fContext = EGL_NO_CONTEXT;
    fSurface = EGL_NO_SURFACE;
#endif    
#endif

	fExpectedDeviceType = kRaster_DeviceType;
#if SK_SUPPORT_GPU
	fExpectedDeviceType = kGPU_DeviceType;
	origin_ = kBottomLeft_GrSurfaceOrigin;
#if SK_ANGLE
	fExpectedDeviceType = kANGLE_DeviceTypeD3D11;
	origin_ = kTopLeft_GrSurfaceOrigin;
#endif
#endif
}

SkOSWindow::~SkOSWindow() 
{
	globalUnInit();

#if SK_SUPPORT_GPU
	SkSafeUnref(fCurContext);
	SkSafeUnref(fCurIntf);

    if (NULL != fHGLRC) {
        wglDeleteContext((HGLRC)fHGLRC);
    }

#if SK_ANGLE
    if (EGL_NO_CONTEXT != fContext) {
        eglDestroyContext(fDisplay, fContext);
        fContext = EGL_NO_CONTEXT;
    }

    if (EGL_NO_SURFACE != fSurface) {
        eglDestroySurface(fDisplay, fSurface);
        fSurface = EGL_NO_SURFACE;
    }

    if (EGL_NO_DISPLAY != fDisplay) {
        eglTerminate(fDisplay);
        fDisplay = EGL_NO_DISPLAY;
    }
#endif // SK_ANGLE
#endif // SK_SUPPORT_GPU
}

void SkOSWindow::setDeviceType(SkWindow::DeviceType type)
{
	if (type == fActualUsedDeviceType) {
		return;
	}
	
	tearDownBackend();
	fExpectedDeviceType = type;
	setUpBackend();

	::InvalidateRect((HWND)fHWND, NULL, TRUE);
}
SkWindow::DeviceType SkOSWindow::getDeviceType()
{
	return fActualUsedDeviceType;
}

void SkOSWindow::init()
{
	setUpBackend();

	if (this->height() && this->width()) {
		this->onSizeChange();
	}
}
void SkOSWindow::unInit()
{
	tearDownBackend();
}


void SkOSWindow::doPaint(void* ctx) {
	bool need_output_bitmap = false;
	StartEndLog log_doPaint("doPaint");
	{
		sk_sp<SkSurface> surface(this->makeSurface());
		SkCanvas* canvas = surface->getCanvas();

		SkAutoCanvasRestore acr(canvas, true);

		this->draw(canvas);

		// TODO: need to fix the problem that opengl and angle paint nothing
		if (kRaster_DeviceType != fActualUsedDeviceType && (need_output_bitmap || layered_)) {
			canvas->readPixels(fBitmap, 0, 0);
		}
	}

	// 输出图片
	// need output bitmap	
	if (need_output_bitmap)
	{
		std::string device_type;
		switch (fActualUsedDeviceType)
		{
		case kRaster_DeviceType:
			device_type = "BITMAP";
			break;
#if SK_SUPPORT_GPU
		case kGPU_DeviceType:
			device_type = "OpenGL";
			break;
#if SK_ANGLE
		case kANGLE_DeviceTypeD3D11:
			device_type = "ANGLE-D3D11";
			break;
		case kANGLE_DeviceTypeD3D9:
			device_type = "ANGLE-D3D9";
			break;
#endif // SK_ANGLE
#endif // SK_SUPPORT_GPU
		default:
			device_type = "";
		}

		const SkBitmap& bitmap = this->getBitmap();
		static int count = 0;
		count++;
		std::stringstream ss;
		ss << "skiashot\\skia_" << device_type << "_" << fHWND << "_" << count << ".png";
		std::string filename = ss.str();
		SkFILEWStream file(filename.c_str());

		if (!SkEncodeImage(&file, bitmap, SkEncodedImageFormat::kPNG, 100)) {

		}
	}

    if (kRaster_DeviceType == fActualUsedDeviceType || layered_)
    {
        HDC hdc = (HDC)ctx;
        const SkBitmap& bitmap = this->getBitmap();

        BITMAPINFO bmi;
        memset(&bmi, 0, sizeof(bmi));
        bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth       = bitmap.width();
        bmi.bmiHeader.biHeight      = -bitmap.height(); // top-down image
        bmi.bmiHeader.biPlanes      = 1;
        bmi.bmiHeader.biBitCount    = 32;
        bmi.bmiHeader.biCompression = BI_RGB;
        bmi.bmiHeader.biSizeImage   = 0;

        //
        // Do the SetDIBitsToDevice.
        //
        // TODO(wjmaclean):
        //       Fix this call to handle SkBitmaps that have rowBytes != width,
        //       i.e. may have padding at the end of lines. The SkASSERT below
        //       may be ignored by builds, and the only obviously safe option
        //       seems to be to copy the bitmap to a temporary (contiguous)
        //       buffer before passing to SetDIBitsToDevice().
        SkASSERT(bitmap.width() * bitmap.bytesPerPixel() == bitmap.rowBytes());
        int ret = SetDIBitsToDevice(hdc,
            0, 0,
            bitmap.width(), bitmap.height(),
            0, 0,
            0, bitmap.height(),
            bitmap.getPixels(),
            &bmi,
            DIB_RGB_COLORS);
        (void)ret; // we're ignoring potential failures for now.

		DWORD err = GetLastError();
		//OutputDebugStringA("SetDIBitsToDevice\n");
    }
}

void SkOSWindow::draw(SkCanvas* canvas)
{
	{
		StartEndLog log_draw("draw");
		SkWindow::draw(canvas);
	}

	{
		StartEndLog log_canvas_flush("canvas->flush");
		canvas->flush();
	}

	{
		StartEndLog log_publish_canvas("publishCanvas");
		// do this last
		publishCanvas(canvas);
	}

	switch (fActualUsedDeviceType)
	{
	case kRaster_DeviceType:
		OutputDebugStringA("use Bitmap\n");
		break;
#if SK_SUPPORT_GPU
	case kGPU_DeviceType:
		OutputDebugStringA("use OpenGL\n");
		break;
#if SK_ANGLE
	case kANGLE_DeviceTypeD3D11:
		OutputDebugStringA("use ANGLE - D3D11\n");
		break;
	case kANGLE_DeviceTypeD3D9:
		OutputDebugStringA("use ANGLE - D3D9\n");
		break;
#endif // SK_ANGLE
#endif // SK_SUPPORT_GPU
	default:
		OutputDebugStringA("default\n");
	}
}

bool SkOSWindow::attach()
{
	bool result = false;

	fActualUsedDeviceType = kRaster_DeviceType;

	switch (fExpectedDeviceType) {
	case kRaster_DeviceType:
		result = true;
		break;
#if SK_SUPPORT_GPU
	case kGPU_DeviceType:
		result = attachGL();
		break;
#if SK_ANGLE
	case kANGLE_DeviceTypeD3D11:
	case kANGLE_DeviceTypeD3D9:
		result = attachANGLE();
		break;
#endif // SK_ANGLE
#endif // SK_SUPPORT_GPU
	default:
		SkASSERT(false);
		result = false;
		break;
	}

	if (result) {
		fActualUsedDeviceType = fExpectedDeviceType;
	}

	return result;
}

void SkOSWindow::detach() {
	switch (fActualUsedDeviceType) {
	case kRaster_DeviceType:
		break;
#if SK_SUPPORT_GPU
	case kGPU_DeviceType:
		detachGL();
		break;
#if SK_ANGLE
	case kANGLE_DeviceTypeD3D11:
	case kANGLE_DeviceTypeD3D9:
		detachANGLE();
		break;
#endif // SK_ANGLE
#endif // SK_SUPPORT_GPU
	default:
		SkASSERT(false);
		break;
	}
	fActualUsedDeviceType = kRaster_DeviceType;
}

void SkOSWindow::present() {
	if (!layered_) {
		switch (fActualUsedDeviceType) {
		case kRaster_DeviceType:
			// nothing to do
			return;
#if SK_SUPPORT_GPU
		case kGPU_DeviceType:
			presentGL();
			break;
#if SK_ANGLE
		case kANGLE_DeviceTypeD3D11:
		case kANGLE_DeviceTypeD3D9:
			presentANGLE();
			break;
#endif // SK_ANGLE
#endif // SK_SUPPORT_GPU
		default:
			SkASSERT(false);
			break;
		}
	}
}

void SkOSWindow::setUpBackend() {
	bool result = attach();

	if (!result) {
		OutputDebugStringA("Failed to initialize GL\n");
		return;
	}

#if SK_SUPPORT_GPU
	fMSAASampleCount = 0;
	fDeepColor = false;

	SkASSERT(NULL == fCurIntf);


	switch (fExpectedDeviceType) {
	case kRaster_DeviceType:
		break;
	case kGPU_DeviceType:
		fCurIntf = GrGLCreateNativeInterface();
		break;
#if SK_ANGLE
	case kANGLE_DeviceTypeD3D11:
	case kANGLE_DeviceTypeD3D9:
		fCurIntf = sk_gpu_angle::CreateANGLEGLInterface();
		break;
#endif // SK_ANGLE
	default:
		SkASSERT(false);
		break;
	}


	if (kRaster_DeviceType != fActualUsedDeviceType)
	{
		SkASSERT(nullptr == fCurContext);
		GrContextOptions fGrContextOptions;
		fGrContextOptions.fGpuPathRenderers = GrContextOptions::GpuPathRenderers::kAll;

		fCurContext = GrContext::MakeGL(fCurIntf, fGrContextOptions).release();

		if (nullptr == fCurContext || nullptr == fCurIntf) {
			// We need some context and interface to see results
			SkSafeUnref(fCurContext);
			SkSafeUnref(fCurIntf);
			fCurContext = nullptr;
			fCurIntf = nullptr;
			OutputDebugStringA("Failed to setup 3D\n");

			detach();
		}
	}
#endif // SK_SUPPORT_GPU
	// call windowSizeChanged to create the render target
	this->onSizeChange();

	onSetTitle();
}

void SkOSWindow::tearDownBackend() {
#if SK_SUPPORT_GPU
	if (fCurContext) {
		// in case we have outstanding refs to this guy (lua?)
		fCurContext->abandonContext();
		fCurContext->unref();
		fCurContext = nullptr;
	}

	SkSafeUnref(fCurIntf);
	fCurIntf = NULL;

	fGpuSurface = nullptr;
#endif

	this->detach();
	fActualUsedDeviceType = kRaster_DeviceType;
}

void SkOSWindow::publishCanvas(SkCanvas* renderingCanvas) {
	present();
}

void SkOSWindow::onSizeChange() {
#if SK_SUPPORT_GPU
	if (fCurContext) {
		attach();

		fActualColorBits = SkTMax(fActualColorBits, 24);
		int w = (int)(width());
		int h = (int)(height());
#if SK_SUPPORT_GPU
		if (fActualUsedDeviceType == kGPU_DeviceType)
			origin_ = kBottomLeft_GrSurfaceOrigin;
#if SK_ANGLE
		if (fActualUsedDeviceType == kANGLE_DeviceTypeD3D11)
			origin_ = kTopLeft_GrSurfaceOrigin;
		if (fActualUsedDeviceType == kANGLE_DeviceTypeD3D9)
			origin_ = kBottomLeft_GrSurfaceOrigin;
#endif
#endif
		fGpuSurface = sk_gpu_angle::makeGpuBackedSurface(fMSAASampleCount, 0, 0, w, h, info(),
			getSurfaceProps(), fCurIntf, fCurContext, origin_);
	}
#endif
}

void SkOSWindow::onDrawTitle(SkCanvas * canvas)
{

}
void SkOSWindow::onSetTitle()
{
	bool caption_system = false;
	LONG styleValue = ::GetWindowLong((HWND)fHWND, GWL_STYLE);
	if ((styleValue & WS_CAPTION) == WS_CAPTION) {
		caption_system = true;
	}

	if (need_get_origin_title_) {
		if (caption_system) {
			char caption_text[MAX_PATH] = { 0 };
			GetWindowTextA((HWND)fHWND, caption_text, MAX_PATH - 1);
			origin_title_ = caption_text;
			need_get_origin_title_ = false;
		}
	}

	switch (fActualUsedDeviceType)
	{
	case kRaster_DeviceType:
		current_title_ = origin_title_ + " : use Bitmap";
		break;
#if SK_SUPPORT_GPU
	case kGPU_DeviceType:
		current_title_ = origin_title_ + " : use OpenGL";
		break;
#if SK_ANGLE
	case kANGLE_DeviceTypeD3D11:
		current_title_ = origin_title_ + " : use ANGLE - D3D11";
		break;
	case kANGLE_DeviceTypeD3D9:
		current_title_ = origin_title_ + " : use ANGLE - D3D9";
		break;
#endif // SK_ANGLE
#endif // SK_SUPPORT_GPU
	default:
		current_title_ = origin_title_;
	}

	if (caption_system) {
		SetWindowTextA((HWND)fHWND, current_title_.c_str());
	}
}

#if SK_SUPPORT_GPU
sk_sp<SkSurface> SkOSWindow::makeGPUSurface() {
	if (IsGpuDeviceType(fActualUsedDeviceType) && fCurContext) {
		SkSurfaceProps props(getSurfaceProps());
		if (kRGBA_F16_SkColorType == this->info().colorType() || fActualColorBits > 24)
		{
			return SkSurface::MakeRenderTarget(fCurContext, SkBudgeted::kNo, this->info(),
				fMSAASampleCount, &props);

		}
		else {
			return fGpuSurface;
		}
	}
	return nullptr;
}

bool SkOSWindow::attachGL() 
{
    HDC dc = GetDC((HWND)fHWND);
    if (NULL == fHGLRC) {
		fHGLRC = SkCreateWGLContext(dc, fMSAASampleCount, fDeepColor,
			kGLPreferCompatibilityProfile_SkWGLContextRequest);
        if (NULL == fHGLRC) {
            return false;
        }
        glClearStencil(0);
        glClearColor(0, 0, 0, 0);
        glStencilMask(0xffffffff);
        glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    }
    if (wglMakeCurrent(dc, (HGLRC)fHGLRC)) {
        // use DescribePixelFormat to get the stencil bit depth.
        int pixelFormat = GetPixelFormat(dc);
        PIXELFORMATDESCRIPTOR pfd;
        DescribePixelFormat(dc, pixelFormat, sizeof(pfd), &pfd);
        fActualColorBits = pfd.cStencilBits;

        // Get sample count if the MSAA WGL extension is present
        SkWGLExtensions extensions;
        if (extensions.hasExtension(dc, "WGL_ARB_multisample")) {
            static const int kSampleCountAttr = SK_WGL_SAMPLES;
            extensions.getPixelFormatAttribiv(dc,
                                              pixelFormat,
                                              0,
                                              1,
                                              &kSampleCountAttr,
                                              &fMSAASampleCount);
        } else {
			fActualColorBits = 0;
        }

        glViewport(0, 0,
                   SkScalarRoundToInt(this->width()),
                   SkScalarRoundToInt(this->height()));
        return true;
    }
    return false;
}

void SkOSWindow::detachGL() {
    wglMakeCurrent(GetDC((HWND)fHWND), 0);
    wglDeleteContext((HGLRC)fHGLRC);
    fHGLRC = NULL;
}

void SkOSWindow::presentGL() {
    glFlush();
    HDC dc = GetDC((HWND)fHWND);
    SwapBuffers(dc);
    ReleaseDC((HWND)fHWND, dc);
}

#if SK_ANGLE

static void* get_angle_egl_display(void* nativeDisplay) {
	PFNEGLGETPLATFORMDISPLAYEXTPROC eglGetPlatformDisplayEXT;
	eglGetPlatformDisplayEXT =
		(PFNEGLGETPLATFORMDISPLAYEXTPROC)eglGetProcAddress("eglGetPlatformDisplayEXT");

	// We expect ANGLE to support this extension
	if (!eglGetPlatformDisplayEXT) {
		return EGL_NO_DISPLAY;
	}

	EGLDisplay display = EGL_NO_DISPLAY;
	// Try for an ANGLE D3D11 context, fall back to D3D9, and finally GL.
	EGLint attribs[3][3] = {
		{
			EGL_PLATFORM_ANGLE_TYPE_ANGLE,
			EGL_PLATFORM_ANGLE_TYPE_D3D11_ANGLE,
			EGL_NONE
		},
		{
			EGL_PLATFORM_ANGLE_TYPE_ANGLE,
			EGL_PLATFORM_ANGLE_TYPE_D3D9_ANGLE,
			EGL_NONE
		},
	};
	for (int i = 0; i < 3 && display == EGL_NO_DISPLAY; ++i) {
		display = eglGetPlatformDisplayEXT(EGL_PLATFORM_ANGLE_ANGLE, nativeDisplay, attribs[i]);
	}
	return display;
}

struct ANGLEAssembleContext {
	ANGLEAssembleContext() {
		fEGL = GetModuleHandleA("libEGL.dll");
		fGL = GetModuleHandleA("libGLESv2.dll");
	}

	bool isValid() const { return SkToBool(fEGL) && SkToBool(fGL); }

	HMODULE fEGL;
	HMODULE fGL;
};

static GrGLFuncPtr angle_get_gl_proc(void* ctx, const char name[]) {
	const ANGLEAssembleContext& context = *reinterpret_cast<const ANGLEAssembleContext*>(ctx);
	GrGLFuncPtr proc = (GrGLFuncPtr)GetProcAddress(context.fGL, name);
	if (proc) {
		return proc;
	}
	proc = (GrGLFuncPtr)GetProcAddress(context.fEGL, name);
	if (proc) {
		return proc;
	}
	return eglGetProcAddress(name);
}

static const GrGLInterface* get_angle_gl_interface() {
	ANGLEAssembleContext context;
	if (!context.isValid()) {
		return nullptr;
	}
	return GrGLAssembleGLESInterface(&context, angle_get_gl_proc);
}

static void SkOSWindowWin_LogError(angle::PlatformMethods *platform, const char *errorMessage)
{
	LogMessage(errorMessage);
	LogNewLine();
}

static void SkOSWindowWin_LogWarning(angle::PlatformMethods *platform, const char *warningMessage)
{
	LogMessage(warningMessage);
	LogNewLine();
}

static void SkOSWindowWin_LogInfo(angle::PlatformMethods *platform, const char *infoMessage)
{
	LogMessage(infoMessage);
	LogNewLine();
}

static const unsigned char * SkOSWindowWin_GetTraceCategoryEnabledFlag(angle::PlatformMethods *platform, const char *categoryName)
{
    static unsigned char enabled = 1;
    return &enabled;
}
static double SkOSWindowWin_MonotonicallyIncreasingTime(angle::PlatformMethods *platform)
{
    return 1.0;
}
static angle::TraceEventHandle SkOSWindowWin_AddTraceEvent(angle::PlatformMethods *platform,
    char phase,
    const unsigned char *categoryEnabledFlag,
    const char *name,
    unsigned long long id,
    double timestamp,
    int numArgs,
    const char **argNames,
    const unsigned char *argTypes,
    const unsigned long long *argValues,
    unsigned char flags)
{

    return SkEventTracer::GetInstance()->addTraceEvent(phase, categoryEnabledFlag,
        name, id, numArgs, argNames, argTypes, argValues, flags);
}

static angle::PlatformMethods* platformMethods()
{
	static struct angle::PlatformMethods methods;

	if (methods.logError != &SkOSWindowWin_LogError) {
		methods.logError = &SkOSWindowWin_LogError;
	}
	if (methods.logWarning != &SkOSWindowWin_LogWarning) {
		methods.logWarning = &SkOSWindowWin_LogWarning;
	}
    if (methods.getTraceCategoryEnabledFlag != &SkOSWindowWin_GetTraceCategoryEnabledFlag) {
        methods.getTraceCategoryEnabledFlag = &SkOSWindowWin_GetTraceCategoryEnabledFlag;
    }
    if (methods.monotonicallyIncreasingTime != &SkOSWindowWin_MonotonicallyIncreasingTime) {
        methods.monotonicallyIncreasingTime = &SkOSWindowWin_MonotonicallyIncreasingTime;
    }
    if (methods.addTraceEvent != &SkOSWindowWin_AddTraceEvent) {
        methods.addTraceEvent = &SkOSWindowWin_AddTraceEvent;
    }

	return &methods;
}

bool create_ANGLE(EGLNativeWindowType hWnd,
                  int msaaSampleCount,
                  EGLDisplay* eglDisplay,
                  EGLContext* eglContext,
                  EGLSurface* eglSurface,
                  EGLConfig* eglConfig, SkWindow::DeviceType angle_type = SkWindow::kANGLE_DeviceTypeD3D11) {
    static const EGLint contextAttribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE, EGL_NONE
    };
    static const EGLint configAttribList[] = {
        EGL_RED_SIZE,       8,
        EGL_GREEN_SIZE,     8,
        EGL_BLUE_SIZE,      8,
        EGL_ALPHA_SIZE,     8,
        EGL_DEPTH_SIZE,     8,
        EGL_STENCIL_SIZE,   8,
        EGL_NONE
    };
	static const EGLint surfaceAttribList[2][4] = {
		{
			EGL_SURFACE_ORIENTATION_ANGLE, EGL_SURFACE_ORIENTATION_INVERT_Y_ANGLE,
			EGL_NONE, EGL_NONE
		},
		{
			EGL_NONE, EGL_NONE,
			EGL_NONE, EGL_NONE
		},
	};
	EGLint attribs[2][5] = {
		{
			EGL_PLATFORM_ANGLE_TYPE_ANGLE,
			EGL_PLATFORM_ANGLE_TYPE_D3D11_ANGLE,
			EGL_NONE
		},
		{
			EGL_PLATFORM_ANGLE_TYPE_ANGLE,
			EGL_PLATFORM_ANGLE_TYPE_D3D9_ANGLE,
			EGL_NONE
		},
	};

	PFNEGLGETPLATFORMDISPLAYEXTPROC eglGetPlatformDisplayEXT;
	eglGetPlatformDisplayEXT =
		(PFNEGLGETPLATFORMDISPLAYEXTPROC)eglGetProcAddress("eglGetPlatformDisplayEXT");
	if (!eglGetPlatformDisplayEXT) {
		return EGL_NO_DISPLAY;
	}

	EGLDisplay display = EGL_NO_DISPLAY;
	if (SkWindow::kANGLE_DeviceTypeD3D11 == angle_type) {
		display = eglGetPlatformDisplayEXT(EGL_PLATFORM_ANGLE_ANGLE, GetDC(hWnd), attribs[0]);
	}
	else if (SkWindow::kANGLE_DeviceTypeD3D9 == angle_type) {
		display = eglGetPlatformDisplayEXT(EGL_PLATFORM_ANGLE_ANGLE, GetDC(hWnd), attribs[1]);
	}
    
    if (display == EGL_NO_DISPLAY ) {
       return false;
    }

	registerPlatformMethods(display, platformMethods());

	// Initialize EGL
	{
		StartEndLog log_eglInitialize("eglInitialize");
#if 0
		LARGE_INTEGER timeStart;	//开始时间
		LARGE_INTEGER timeEnd;		//结束时间
		LARGE_INTEGER frequency;	//计时器频率
		QueryPerformanceFrequency(&frequency);
		double quadpart = (double)frequency.QuadPart;//计时器频率
		QueryPerformanceCounter(&timeStart);
		// Initialize EGL
		EGLint majorVersion, minorVersion;
		if (!eglInitialize(display, &majorVersion, &minorVersion)) {
			return false;
		}
		QueryPerformanceCounter(&timeEnd);
		double elapsed = (timeEnd.QuadPart - timeStart.QuadPart) / quadpart;
		elapsed *= 1000;
		{
			std::stringstream ss;
			ss << "eglInitialize cosuming: " << elapsed << std::endl;
			LogMessage(ss.str().c_str());
		}
#else 
		// Initialize EGL
		EGLint majorVersion, minorVersion;
		if (!eglInitialize(display, &majorVersion, &minorVersion)) {
			EGLint error = eglGetError();
			std::stringstream ss;
			ss << "End eglInitialize failed. Error Code : " << error << std::endl;
			LogMessage(ss.str().c_str());
			return false;
		}
#endif
	}
	
    EGLint numConfigs;
    if (!eglGetConfigs(display, NULL, 0, &numConfigs)) {
       return false;
    }

    // Choose config
    bool foundConfig = false;
    if (msaaSampleCount) {
        static const int kConfigAttribListCnt =
                                SK_ARRAY_COUNT(configAttribList);
        EGLint msaaConfigAttribList[kConfigAttribListCnt + 4];
        memcpy(msaaConfigAttribList,
               configAttribList,
               sizeof(configAttribList));
        SkASSERT(EGL_NONE == msaaConfigAttribList[kConfigAttribListCnt - 1]);
        msaaConfigAttribList[kConfigAttribListCnt - 1] = EGL_SAMPLE_BUFFERS;
        msaaConfigAttribList[kConfigAttribListCnt + 0] = 1;
        msaaConfigAttribList[kConfigAttribListCnt + 1] = EGL_SAMPLES;
        msaaConfigAttribList[kConfigAttribListCnt + 2] = msaaSampleCount;
        msaaConfigAttribList[kConfigAttribListCnt + 3] = EGL_NONE;
        if (eglChooseConfig(display, configAttribList, eglConfig, 1, &numConfigs)) {
            SkASSERT(numConfigs > 0);
            foundConfig = true;
        }
    }
    if (!foundConfig) {
        if (!eglChooseConfig(display, configAttribList, eglConfig, 1, &numConfigs)) {
           return false;
        }
    }

    // Create a surface
	EGLSurface surface = EGL_NO_SURFACE;
	if (SkWindow::kANGLE_DeviceTypeD3D11 == angle_type) {
		surface = eglCreateWindowSurface(display, *eglConfig,
			(EGLNativeWindowType)hWnd,
			surfaceAttribList[0]);
	}
	else if (SkWindow::kANGLE_DeviceTypeD3D9 == angle_type) {
		surface = eglCreateWindowSurface(display, *eglConfig,
			(EGLNativeWindowType)hWnd,
			surfaceAttribList[1]);
	}
    if (surface == EGL_NO_SURFACE) {
       return false;
    }

    // Create a GL context
    EGLContext context = eglCreateContext(display, *eglConfig,
                                          EGL_NO_CONTEXT,
                                          contextAttribs );
    if (context == EGL_NO_CONTEXT ) {
       return false;
    }

    // Make the context current
    if (!eglMakeCurrent(display, surface, surface, context)) {
       return false;
    }

    *eglDisplay = display;
    *eglContext = context;
    *eglSurface = surface;
    return true;
}

bool SkOSWindow::attachANGLE() {
    if (EGL_NO_DISPLAY == fDisplay) {
        bool bResult = create_ANGLE((HWND)fHWND,
                                    fMSAASampleCount,
                                    &fDisplay,
                                    &fContext,
                                    &fSurface,
                                    &fConfig, fExpectedDeviceType);
        if (false == bResult) {
            return false;
        }
		fANGLEInterface.reset(get_angle_gl_interface());
		if (!fANGLEInterface) {
			this->detachANGLE();
			return false;
		}
		GL_CALL(fANGLEInterface, ClearStencil(0));
		GL_CALL(fANGLEInterface, ClearColor(0, 0, 0, 0));
		GL_CALL(fANGLEInterface, StencilMask(0xffffffff));
		GL_CALL(fANGLEInterface, Clear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT));
	}
	if (!eglMakeCurrent(fDisplay, fSurface, fSurface, fContext)) {
		this->detachANGLE();
		return false;
	}
	eglGetConfigAttrib(fDisplay, fConfig, EGL_STENCIL_SIZE, &fActualColorBits);
	eglGetConfigAttrib(fDisplay, fConfig, EGL_SAMPLES, &fMSAASampleCount);

	GL_CALL(fANGLEInterface, Viewport(0, 0, SkScalarRoundToInt(this->width()),
		SkScalarRoundToInt(this->height())));
	return true;
}

void SkOSWindow::detachANGLE() {
    eglMakeCurrent(fDisplay, EGL_NO_SURFACE , EGL_NO_SURFACE , EGL_NO_CONTEXT);

    eglDestroyContext(fDisplay, fContext);
    fContext = EGL_NO_CONTEXT;

    eglDestroySurface(fDisplay, fSurface);
    fSurface = EGL_NO_SURFACE;

    eglTerminate(fDisplay);
    fDisplay = EGL_NO_DISPLAY;
}

void SkOSWindow::presentANGLE() {
	GL_CALL(fANGLEInterface, Flush());

	EGLBoolean ret = eglSwapBuffers(fDisplay, fSurface);
	if (!ret) {
		EGLint error = eglGetError();
		if (error != EGL_SUCCESS) {
			std::stringstream ss;
			ss << "presentANGLE egl error: " << error << "\n";
			OutputDebugStringA(ss.str().c_str());
		}
	}
}
#endif // SK_ANGLE
#endif // SK_SUPPORT_GPU



sk_sp<SkSurface> SkOSWindow::makeSurface()
{
	sk_sp<SkSurface> surface;
#if SK_SUPPORT_GPU
	if (IsGpuDeviceType(fExpectedDeviceType)) {
		surface = makeGPUSurface();
	}
#endif
	if (!surface) {
		surface = SkWindow::makeSurface();
	}
	return surface;
}

