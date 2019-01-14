#include "FrameWindowWnd.h"
#include "resource.h"

#include "skwin\SkWindow.h"

#include "SkTypes.h"

#include "SkTypeface.h"

#include <sstream>

#include <WindowsX.h>

#include "codec\SkCodec.h"
#include "encode\SkEncoder.h"

#include <stdio.h>
#include <strsafe.h>
#include <tchar.h>
#include <windows.h>

// timer id
#define TIMER_ID 100

#define TEST_DECODE_SCALE_JPG 0

static inline bool decode_file(const char* filename, SkBitmap* bitmap, double scale = 1.0,
                               SkColorType colorType = kN32_SkColorType,
                               bool requireUnpremul = false) {
    sk_sp<SkData> data(SkData::MakeFromFileName(filename));
    std::unique_ptr<SkCodec> codec = SkCodec::MakeFromData(data);
    if (!codec) {
        return false;
    }

    SkISize size = codec->getScaledDimensions(scale);

    const SkImageInfo& imgInfo = codec->getInfo();
    auto alphaType = imgInfo.isOpaque() ? kOpaque_SkAlphaType : kPremul_SkAlphaType;
    const SkImageInfo info =
            SkImageInfo::Make(size.width(), size.height(), kN32_SkColorType, alphaType);

    bitmap->allocPixels(info);

    return SkCodec::kSuccess == codec->getPixels(info, bitmap->getPixels(), bitmap->rowBytes());
}

CFrameWindowWnd::CFrameWindowWnd(void)
    : paint_window_(nullptr)
    , layered_(false)
    , first_size_changed_(false) 
{
    caption_height_ = ::GetSystemMetrics(SM_CYCAPTION);
    hittest_caption_ = {0, 0, 0, caption_height_};
    hittest_sizebox_ = {2, 2, 2, 2};
    caption_system_ = false;
    anti_alias_ = true;
    lcd_text_ = true;
    bitmap_alpha_type_ = 1;
}
CFrameWindowWnd::~CFrameWindowWnd(void) {}

void CFrameWindowWnd::setDeviceType(int type) {
    if (paint_window_) {
        paint_window_->setDeviceType(SkWindow::DeviceType(type));
    }
}
int CFrameWindowWnd::getDeviceType() {
    if (paint_window_) {
        int type = paint_window_->getDeviceType();
        return type;
    }
    return -1;
}

LPCTSTR CFrameWindowWnd::GetWindowClassName() const { return TEXT("UIMainFrame"); };

UINT CFrameWindowWnd::GetClassStyle() const { return UI_CLASSSTYLE_FRAME; };

void CFrameWindowWnd::OnFinalMessage(HWND /*hWnd*/) { delete this; }

LRESULT CFrameWindowWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LRESULT lRes = 0;
    BOOL bHandled = TRUE;
    switch (uMsg) {
        case WM_NCCREATE:
            lRes = OnNcCreate(uMsg, wParam, lParam, bHandled);
            break;
        case WM_CREATE:
            lRes = OnCreate(uMsg, wParam, lParam, bHandled);
            break;
        case WM_CLOSE:
            lRes = OnClose(uMsg, wParam, lParam, bHandled);
            break;
        case WM_DESTROY:
            lRes = OnDestroy(uMsg, wParam, lParam, bHandled);
            break;
        case WM_NCACTIVATE:
            lRes = OnNcActivate(uMsg, wParam, lParam, bHandled);
            break;
        case WM_NCCALCSIZE:
            lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled);
            break;
        case WM_NCPAINT:
            lRes = OnNcPaint(uMsg, wParam, lParam, bHandled);
            break;
        case WM_NCHITTEST:
            lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled);
            break;
        case WM_SIZE:
            lRes = OnSize(uMsg, wParam, lParam, bHandled);
            break;
        case WM_PAINT:
            lRes = OnPaint(uMsg, wParam, lParam, bHandled);
            break;
        case WM_GETMINMAXINFO:
            lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled);
            break;
        case WM_SYSCOMMAND:
            lRes = OnSysCommand(uMsg, wParam, lParam, bHandled);
            break;
        case WM_TIMER:
            lRes = OnTimer(uMsg, wParam, lParam, bHandled);
            break;
        case WM_KEYDOWN:
            lRes = OnKeyDown(uMsg, wParam, lParam, bHandled);
            break;
        case WM_ERASEBKGND:
            return 1;
        default:
            bHandled = FALSE;
    }
    if (bHandled) return lRes;
    return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

#define TEST_DRAW_FLOW 1
void CFrameWindowWnd::drawContent(SkCanvas* canvas) {
#if TEST_DRAW_FLOW
    SkColor bkcolor = SkColorSetARGB(0xe0, 0xee, 0xee, 0xee);
    canvas->clear(bkcolor);
#else
    SkColor bkcolor = SkColorSetARGB(0xe0, 0xee, 0xee, 0xee);
    canvas->clear(bkcolor);

    // draw layered headers
    drawLayeredHeader(canvas);

    // draw text
    testDrawText(canvas);

    // draw image
    SkBitmap image;
    decode_file("website.jpg", &image);
    canvas->drawBitmap(image, 100, 120);
#endif
}

//
LRESULT CFrameWindowWnd::OnNcCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
    // LPCREATESTRUCTA create_info = (LPCREATESTRUCTA)lParam;
    AjustLayeredAndCaption();
    bHandled = FALSE;
    return 0;
}

LRESULT CFrameWindowWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
    SetIcon(IDI_ICON1);
    // AjustLayeredAndCaption();
    paint_window_ = new SkOSWindow(m_hWnd);
    if (paint_window_) {
        paint_window_->setDraw(this);
        paint_window_->setLayered(layered_);
        paint_window_->init();
    }
    ::SetTimer(m_hWnd, TIMER_ID, 1000, 0);
    bHandled = FALSE;
    return 0;
}
LRESULT CFrameWindowWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
    bHandled = FALSE;
    return 0;
}
LRESULT CFrameWindowWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
    ::KillTimer(m_hWnd, TIMER_ID);
    if (paint_window_) {
        paint_window_->unInit();
        paint_window_->unref();
    }
    ::PostQuitMessage(0L);

    bHandled = FALSE;
    return 0;
}
LRESULT CFrameWindowWnd::OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
    if (::IsIconic(*this)) bHandled = FALSE;
    return (wParam == 0) ? TRUE : FALSE;
}
LRESULT CFrameWindowWnd::OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
    // AjustLayeredAndCaption();

    if (caption_system_) {
        bHandled = FALSE;
    }
    return 0;
}
LRESULT CFrameWindowWnd::OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
    if (caption_system_) {
        bHandled = FALSE;
    }
    return 0;
}
LRESULT CFrameWindowWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
    if (!caption_system_) {
        POINT pt;
        pt.x = GET_X_LPARAM(lParam);
        pt.y = GET_Y_LPARAM(lParam);
        ::ScreenToClient(*this, &pt);

        RECT rcClient;
        ::GetClientRect(*this, &rcClient);

        if (!::IsZoomed(*this)) {
            RECT rcSizeBox = hittest_sizebox_;
            if (pt.y < rcClient.top + rcSizeBox.top) {
                if (pt.x < rcClient.left + rcSizeBox.left) return HTTOPLEFT;
                if (pt.x > rcClient.right - rcSizeBox.right) return HTTOPRIGHT;
                return HTTOP;
            } else if (pt.y > rcClient.bottom - rcSizeBox.bottom) {
                if (pt.x < rcClient.left + rcSizeBox.left) return HTBOTTOMLEFT;
                if (pt.x > rcClient.right - rcSizeBox.right) return HTBOTTOMRIGHT;
                return HTBOTTOM;
            }
            if (pt.x < rcClient.left + rcSizeBox.left) return HTLEFT;
            if (pt.x > rcClient.right - rcSizeBox.right) return HTRIGHT;
        }

        RECT rcCaption = hittest_caption_;
        if (pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right &&
            pt.y >= rcCaption.top && pt.y < rcCaption.bottom) {
            return HTCAPTION;
        }

        return HTCLIENT;
    }

    bHandled = FALSE;

    return 0;
}
LRESULT CFrameWindowWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
    if (!::IsIconic(*this)) {
        first_size_changed_ = true;
        int cx = GET_X_LPARAM(lParam);
        int cy = GET_Y_LPARAM(lParam);
        LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
        if ((styleValue & WS_CAPTION) == WS_CAPTION) {
            OutputDebugStringA("has caption\n");
        }
        RECT rcWnd;
        if (layered_) {
            ::GetWindowRect(*this, &rcWnd);
        } else {
            RECT rcWndTmp;
            ::GetWindowRect(*this, &rcWndTmp);
            ::GetClientRect(*this, &rcWnd);
        }

        int w = rcWnd.right - rcWnd.left;
        int h = rcWnd.bottom - rcWnd.top;

        if (paint_window_) paint_window_->resize(w, h);

        if (layered_) {
            InvalidateRect(m_hWnd, NULL, TRUE);

            if (offscreen_hbmp_) DeleteObject(offscreen_hbmp_);
            if (offscreen_hdc_) DeleteDC(offscreen_hdc_);

            HDC hdc = GetDC((HWND)m_hWnd);
            offscreen_hdc_ = ::CreateCompatibleDC(hdc);
            BITMAPINFO bmi;
            memset(&bmi, 0, sizeof(bmi));
            bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            bmi.bmiHeader.biWidth = w;
            bmi.bmiHeader.biHeight = -h;  // top-down image
            bmi.bmiHeader.biPlanes = 1;
            bmi.bmiHeader.biBitCount = 32;
            bmi.bmiHeader.biCompression = BI_RGB;
            bmi.bmiHeader.biSizeImage = 0;
            BYTE* lpBitmapBits = NULL;
            offscreen_hbmp_ = ::CreateDIBSection(offscreen_hdc_, &bmi, DIB_RGB_COLORS,
                                                 (LPVOID*)&lpBitmapBits, NULL, 0);
            ReleaseDC((HWND)m_hWnd, hdc);
        }
    }
    bHandled = FALSE;
    return 0;
}
LRESULT CFrameWindowWnd::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
    if (!first_size_changed_) {
        PAINTSTRUCT ps = {0};
        HDC hdc = ::BeginPaint(m_hWnd, &ps);
        ::EndPaint(m_hWnd, &ps);
        return 1;
    }
    if (layered_) {
        HBITMAP old_hbmp_ = (HBITMAP)::SelectObject(offscreen_hdc_, offscreen_hbmp_);
        if (paint_window_) paint_window_->doPaint(offscreen_hdc_);

        RECT rtwnd;
        GetWindowRect((HWND)m_hWnd, &rtwnd);
        POINT ptSrc;
        ptSrc.x = 0;
        ptSrc.y = 0;
        SIZE sz;
        sz.cx = rtwnd.right - rtwnd.left;
        sz.cy = rtwnd.bottom - rtwnd.top;
        POINT ptDest;
        ptDest.x = rtwnd.left;
        ptDest.y = rtwnd.top;
        BLENDFUNCTION bf = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};

        HDC hDesktopDc = GetDC(NULL);
        BOOL ret = ::UpdateLayeredWindow((HWND)m_hWnd, hDesktopDc, &ptDest, &sz, offscreen_hdc_,
                                         &ptSrc, 0, &bf, ULW_ALPHA);
        DWORD err = GetLastError();
        ReleaseDC(NULL, hDesktopDc);

        ::SelectObject(offscreen_hdc_, old_hbmp_);

        PAINTSTRUCT ps = {0};
        HDC hdc = ::BeginPaint(m_hWnd, &ps);
        ::EndPaint(m_hWnd, &ps);
    } else {
        PAINTSTRUCT ps = {0};
        HDC hdc = ::BeginPaint(m_hWnd, &ps);
        if (paint_window_) {
            RECT rcClient;
            paint_window_->doPaint(hdc);
        }
        ::EndPaint(m_hWnd, &ps);
    }

    bHandled = TRUE;
    return 1;
}
LRESULT CFrameWindowWnd::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
    int primaryMonitorWidth = ::GetSystemMetrics(SM_CXSCREEN);
    int primaryMonitorHeight = ::GetSystemMetrics(SM_CYSCREEN);
    MONITORINFO oMonitor = {};
    oMonitor.cbSize = sizeof(oMonitor);
    ::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
    RECT rcWork = oMonitor.rcWork;
    ::OffsetRect(&rcWork, -oMonitor.rcMonitor.left, -oMonitor.rcMonitor.top);
    if (rcWork.right > primaryMonitorWidth) rcWork.right = primaryMonitorWidth;
    if (rcWork.bottom > primaryMonitorHeight) rcWork.bottom = primaryMonitorHeight;
    LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
    lpMMI->ptMaxPosition.x = rcWork.left;
    lpMMI->ptMaxPosition.y = rcWork.top;
    lpMMI->ptMaxSize.x = rcWork.right;
    lpMMI->ptMaxSize.y = rcWork.bottom;
    bHandled = FALSE;
    return 0;
}
LRESULT CFrameWindowWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
    if (wParam == SC_CLOSE) {
        ::PostQuitMessage(0L);
        bHandled = TRUE;
        return 0;
    }
    BOOL bZoomed = ::IsZoomed(*this);
    LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
    if (::IsZoomed(*this) != bZoomed) {
        if (!bZoomed) {
        } else {
        }
    }
    return lRes;
}
LRESULT CFrameWindowWnd::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
    if (wParam == TIMER_ID) {
        InvalidateRect(m_hWnd, NULL, FALSE);
    }
    return 0;
}
LRESULT CFrameWindowWnd::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) {
    switch (wParam) {
        case '0':
            setDeviceType(0);
            break;
        case '1':
            setDeviceType(1);
            break;
        case '2':
            setDeviceType(2);
            break;
        case '3':
            setDeviceType(3);
            break;
        case 'a':
        case 'A':
            anti_alias_ = !anti_alias_;
            InvalidateRect(m_hWnd, NULL, TRUE);
            break;
        case 'b':
        case 'B':
            bitmap_alpha_type_++;
            if (bitmap_alpha_type_ == 4) bitmap_alpha_type_ = 1;
            InvalidateRect(m_hWnd, NULL, TRUE);
            break;
        case 'l':
        case 'L':
            lcd_text_ = !lcd_text_;
            InvalidateRect(m_hWnd, NULL, TRUE);
            break;
        case VK_F6: {
#if TEST_DECODE_SCALE_JPG
            {
                OutputDebugStringA("begin \n");
                DWORD dwStart = GetTickCount();
                for (int i = 0; i < 1000; i++) {
                    SkBitmap image;
                    decode_file("yanxi.jpg", &image, 0.01);
                    // canvas->drawBitmap(image, 100, 100);
                }
                DWORD dwEnd = GetTickCount();
                std::stringstream ss;
                ss << "decode jpg 0.1 cosume: " << (dwEnd - dwStart) << "\n";
                OutputDebugStringA(ss.str().c_str());
            }
            {
                DWORD dwStart = GetTickCount();
                for (int i = 0; i < 1000; i++) {
                    SkBitmap image;
                    decode_file("yanxi.jpg", &image);
                    // canvas->drawBitmap(image, 100, 300);
                }
                DWORD dwEnd = GetTickCount();
                std::stringstream ss;
                ss << "decode jpg 1 cosume: " << (dwEnd - dwStart) << " \n";
                OutputDebugStringA(ss.str().c_str());
                OutputDebugStringA("end \n");
            }
#endif
            break;
        }
        default:
            bHandled = FALSE;
            break;
    }
    return 0;
}

void CFrameWindowWnd::AjustLayeredAndCaption() {
    if ((GetWindowLong(m_hWnd, GWL_EXSTYLE) & WS_EX_LAYERED) != 0) {
        layered_ = true;
    } else
        layered_ = false;

    LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
    if ((styleValue & WS_CAPTION) == WS_CAPTION) {
        caption_system_ = true;
    }

    if (layered_) {
        caption_system_ = false;
        styleValue &= ~WS_CAPTION;
        ::SetWindowLong(*this, GWL_STYLE, styleValue);
    }
}
void CFrameWindowWnd::drawLayeredHeader(SkCanvas* canvas) {
    if (layered_) {
        RECT window_rect;
        GetWindowRect(m_hWnd, &window_rect);
        int window_width = window_rect.right - window_rect.left;
        int window_height = window_rect.bottom - window_rect.top;

        canvas->save();
        SkColor board_color = SkColorSetARGB(0xFF, 0xee, 0xee, 0xee);
        SkRect board_rect = SkRect::MakeXYWH(0, 0, window_width, window_height);
        SkPaint board_paint;
        board_paint.setColor(board_color);
        board_paint.setStyle(SkPaint::kStroke_Style);
        board_paint.setStrokeWidth(2);
        canvas->drawRect(board_rect, board_paint);

        SkColor caption_bk_color = SkColorSetARGB(0xFF, 0xee, 0xee, 0xee);
        SkRect caption_rect = SkRect::MakeXYWH(0, 0, window_width, caption_height_);
        SkPaint caption_paint;
        caption_paint.setColor(caption_bk_color);
        caption_paint.setStyle(SkPaint::kFill_Style);
        canvas->drawRect(caption_rect, caption_paint);
        canvas->restore();
    }
}
void CFrameWindowWnd::testDrawText(SkCanvas* canvas) {
    SkPaint paint;
    paint.setLCDRenderText(lcd_text_);
    {
        TCHAR s_fontName[64] = TEXT("΢���ź�");
        char utf8FontName[64] = {0};
        WideCharToMultiByte(CP_UTF8, 0, s_fontName, lstrlen(s_fontName), utf8FontName, 64, NULL, 0);
        sk_sp<SkTypeface> typeface = SkTypeface::MakeFromName(utf8FontName, SkFontStyle::Normal());
        paint.setAntiAlias(anti_alias_);
        paint.setTypeface(typeface);
        paint.setFilterQuality(kHigh_SkFilterQuality);
        SkColor colorText = SkColorSetARGB(0xff, 0x44, 0x7A, 0xA1);
        paint.setColor(colorText);
        paint.setTextSize(14);
    }
    WCHAR render_type[64] = {0};
    switch (getDeviceType()) {
        case 0:
            lstrcpynW(render_type, L"��Ⱦ�豸: BITMAP", 64);
            break;
        case 1:
            lstrcpynW(render_type, L"��Ⱦ�豸: OpenGL", 64);
            break;
        case 2:
            lstrcpynW(render_type, L"��Ⱦ�豸: ANGLE - D3D11", 64);
            break;
        case 3:
            lstrcpynW(render_type, L"��Ⱦ�豸: ANGLE - D3D9", 64);
            break;
        default:
            lstrcpynW(render_type, L"��Ⱦ�豸: δ֪", 64);
            break;
    }
    if (anti_alias_) {
        StringCbCatW(render_type, 64, L" AntiAlias");
    }
    // const WCHAR w_text[64] = L"����, Skia!";
    char utf8_text[256] = {0};
    {
        // WideCharToMultiByte(CP_UTF8, 0, w_text, lstrlenW(w_text), utf8_text, 256, 0, 0);
        WideCharToMultiByte(CP_UTF8, 0, render_type, lstrlenW(render_type), utf8_text, 256, 0, 0);
    }
    canvas->drawText(utf8_text, strlen(utf8_text), 50, 100, paint);
    // drawCurrentGlyphFils(canvas);
    SkBitmap image;
    decode_file("website.jpg", &image);
    canvas->drawBitmap(image, 100, 120);
    return;
}
