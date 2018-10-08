#ifndef SkWindow_DEFINED
#define SkWindow_DEFINED

// start : ignore skia dll warnings
#pragma warning( push )  
#pragma warning( disable : 4251 )  

#if SK_SUPPORT_GPU
#else
class GrContext;
#endif

#include "SkBitmap.h"
#include "SkMatrix.h"
#include "SkRegion.h"
#include "SkString.h"
#include "SkSurface.h"

// end : ignore skia dll warnings
#pragma warning( pop )

#include "WindowDraw.h"

class SkWindow : public SkRefCnt {
public:
	enum DeviceType {
		kRaster_DeviceType = 0,
#if SK_SUPPORT_GPU
		kGPU_DeviceType = 1,
#if SK_ANGLE
		kANGLE_DeviceTypeD3D11 = 2,
		kANGLE_DeviceTypeD3D9 = 3,
		kANGLE_DeviceType = kANGLE_DeviceTypeD3D11,
#endif // SK_ANGLE
#endif // SK_SUPPORT_GPU
		kDeviceTypeCnt
	};

public:
    SkWindow();
    virtual ~SkWindow();

	virtual void setDeviceType(DeviceType type) = 0;
	virtual DeviceType getDeviceType() = 0;


	virtual void init() = 0;
	virtual void unInit() = 0;
	virtual void resize(int width, int height);

	void setLayered(bool bLayered) { layered_ = bLayered; }

	SkImageInfo info() const { return fBitmap.info(); }
    const SkBitmap& getBitmap() const { return fBitmap; }

	virtual sk_sp<SkSurface> makeSurface();

	SkSurfaceProps getSurfaceProps() const { return fSurfaceProps; }

	SkScalar    width() const { return fWidth; }
    SkScalar    height() const { return fHeight; }

	void setDraw(IWindowDraw *draw_ptr) {
		draw_ = draw_ptr;
	}

protected:
	virtual void draw(SkCanvas* canvas);
	virtual void onSizeChange() { };

protected:
	SkScalar    fWidth, fHeight;
	SkSurfaceProps  fSurfaceProps;
    SkBitmap    fBitmap;

	IWindowDraw * draw_;
	bool layered_;

private:
	typedef SkRefCnt INHERITED;
};


#if defined(SK_BUILD_FOR_WIN)
    #include ".\win\SkOSWindow_Win.h"
#else 
#error
#endif

#endif
