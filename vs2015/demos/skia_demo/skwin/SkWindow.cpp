// start : ignore skia dll warnings
#pragma warning( push )  
#pragma warning( disable : 4251 ) 

#include "SkWindow.h"
#include "SkSurface.h"
#include "SkCanvas.h"
#include "SkTime.h"

// end : ignore skia dll warnings
#pragma warning( pop )


SkWindow::SkWindow() 
	: fWidth(0), fHeight(0)
	, fSurfaceProps(SkSurfaceProps::kLegacyFontHost_InitType)
{
}

SkWindow::~SkWindow() 
{
}

sk_sp<SkSurface> SkWindow::makeSurface()
{
	const SkBitmap& bm = this->getBitmap();
	return SkSurface::MakeRasterDirect(bm.info(), bm.getPixels(), bm.rowBytes(), &fSurfaceProps);
}


void SkWindow::resize(int width, int height) {
    if (width != fBitmap.width() || height != fBitmap.height()) {
        fBitmap.allocN32Pixels(width, height, false);
    }
	if (fWidth != width || fHeight != height)
	{
		fWidth = SkIntToScalar(width);
		fHeight = SkIntToScalar(height);
		this->onSizeChange();
	}
}


void SkWindow::draw(SkCanvas* canvas)
{
    if (fWidth && fHeight)
    {
        SkAutoCanvasRestore as(canvas, true);

		int sc = canvas->save();
		{
			canvas->drawColor(SK_ColorWHITE);

			if (draw_) {
				draw_->drawContent(canvas);
			}
		}
        canvas->restoreToCount(sc);
    }
}

