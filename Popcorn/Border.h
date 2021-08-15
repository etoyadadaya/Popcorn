#pragma once

#include "Config.h"

//------------------------------------------------------------------------------------------------------------
class AsBorder
{
public:
	AsBorder();

	void Init();
	void Draw(HDC hdc, RECT &paint_area, HPEN bg_pen, HBRUSH bg_brush);


private:
	void Draw_Element(HDC hdc, int x, int y, bool top_border, HPEN bg_pen, HBRUSH bg_brush);

	HPEN Ball_Pen, Border_Blue_Pen, Border_White_Pen;
	HBRUSH Ball_Brush, Border_Blue_Brush, Border_White_Brush;

};
//------------------------------------------------------------------------------------------------------------