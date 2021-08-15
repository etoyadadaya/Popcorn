﻿#pragma once

#include "Config.h"

//AsPlatform
//------------------------------------------------------------------------------------------------------------
class AsPlatform
{
public:
	AsPlatform();

	void Init();
	void Redraw_Platform(HWND hwnd);
	void Draw(HDC hdc, HPEN bg_pen, HBRUSH bg_brush, RECT &paint_area);

	int X_Pos;
	int Width;
	int X_Step;


private:
	int Inner_Width;

	RECT Platform_Rect, Prev_Platform_Rect;

	HPEN  Highlight_Pen, Platform_Circle_Pen, Platform_Inner_Pen;
	HBRUSH Platform_Circle_Brush, Platform_Inner_Brush;

	static const int Height = 7;
	static const int Circle_Size = 7;
};
//------------------------------------------------------------------------------------------------------------