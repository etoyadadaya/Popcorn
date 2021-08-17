#pragma once

#include "Config.h"

//------------------------------------------------------------------------------------------------------------
class AActive_Brick
{
public:
	AActive_Brick();

	void Act(HWND hwnd);
	void Draw(HDC hdc, RECT &paint_area);

	int Fade_Step;
	RECT Brick_Rect;

	static const int Max_Fade_Step = 40;
	

};
//------------------------------------------------------------------------------------------------------------