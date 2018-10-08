#pragma once

#ifndef __WINDOWDRAW_H__
#define __WINDOWDRAW_H__

class SkCanvas;

class IWindowDraw
{
public:
	virtual void drawContent(SkCanvas * canvas) = 0;
};

#endif//__WINDOW_DRAW_H__
