#ifndef __FRAMEWINDOWWND_H__
#define __FRAMEWINDOWWND_H__

#include <Windows.h>
#include "WindowWnd.h"

#if SK_SUPPORT_GPU
#else
class GrContext;
#endif

#include "SkCanvas.h"

#include "skwin\SkWindow.h"

class CFrameWindowWnd : public CWindowWnd, public IWindowDraw
{
public:
	CFrameWindowWnd(void);
	~CFrameWindowWnd(void);

public:
	/**
	* @param type : 0 bitmap; 1 opengl; 2 angle;
	*/
	void setDeviceType(int type);
	/**
	* @return type : 0 bitmap; 1 opengl; 2 angle;
	*/
	int getDeviceType();

public:
	void drawContent(SkCanvas * canvas);

protected:
	LPCTSTR GetWindowClassName() const;
	UINT GetClassStyle() const;
	void OnFinalMessage(HWND /*hWnd*/);
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	// win32 message handler
	virtual LRESULT OnNcCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
	void AjustLayeredAndCaption();

private:
	void drawLayeredHeader(SkCanvas *canvas);
	void testDrawText(SkCanvas *canvas);
	void drawCurrentGlyphFils(SkCanvas * canvas);

protected:
	SkOSWindow * paint_window_;
	RECT hittest_caption_;
	RECT hittest_sizebox_;
	int caption_height_;
	bool caption_system_;

	bool layered_;
	HBITMAP offscreen_hbmp_;
	HDC offscreen_hdc_;
	bool anti_alias_;
	bool lcd_text_;
	int bitmap_alpha_type_;
    bool first_size_changed_;
};

#endif//__FRAMEWINDOWWND_H__
