#include "Ball.h"

//ABall
const double ABall::Start_Ball_Y_Pos = 181.0;
//------------------------------------------------------------------------------------------------------------	
ABall::ABall()
: Ball_State(EBS_Normal), Ball_Pen(0), Ball_Brush(0), Ball_X_Pos(0.0), Ball_Y_Pos(Start_Ball_Y_Pos), Ball_Speed(0.0), Ball_Direction(0), Ball_Rect{}, Prev_Ball_Rect{}
{
	Set_State(EBS_Normal, 0);
}
//------------------------------------------------------------------------------------------------------------
void ABall::Init()
{
	AsConfig::Create_Pen_Brush(255, 255, 255, Ball_Pen, Ball_Brush);
}
//------------------------------------------------------------------------------------------------------------
void ABall::Draw(HDC hdc, RECT &paint_area)
{
	RECT intersection_rect;

	// 1. Очищаем фон 
	if (IntersectRect(&intersection_rect, &paint_area, &Prev_Ball_Rect) )
	{ 
		SelectObject(hdc, AsConfig::BG_Pen);
		SelectObject(hdc, AsConfig::BG_Brush);

		Ellipse(hdc, Prev_Ball_Rect.left, Prev_Ball_Rect.top, Prev_Ball_Rect.right - 1, Prev_Ball_Rect.bottom - 1);
	}

	// 2. Рисуем шарик
	if (IntersectRect(&intersection_rect, &paint_area, &Ball_Rect) )
	{ 
		SelectObject(hdc, Ball_Pen);
		SelectObject(hdc, Ball_Brush);

		Ellipse(hdc, Ball_Rect.left, Ball_Rect.top, Ball_Rect.right - 1, Ball_Rect.bottom - 1);
	}
}
//------------------------------------------------------------------------------------------------------------
void ABall::Move(ALevel *level, int platform_x_pos, int platform_width)
{
	double next_x_pos, next_y_pos;
	int max_x_pos = AsConfig::Max_X_Pos - AsConfig::Ball_Size;
	int max_y_pos = AsConfig::Max_Y_Pos - AsConfig::Ball_Size;
	int platform_y_pos = AsConfig::Platform_Y_Pos - AsConfig::Ball_Size;

	if (Ball_State != EBS_Normal)
		return;

	Prev_Ball_Rect = Ball_Rect;

	next_x_pos = Ball_X_Pos + Ball_Speed * cos(Ball_Direction);
	next_y_pos = Ball_Y_Pos - Ball_Speed * sin(Ball_Direction);

	// Корректируем позицию при отражении от рамки
	if (next_x_pos < AsConfig::Border_X_Offset)
	{
		next_x_pos = AsConfig::Level_X_Offset - (next_x_pos - AsConfig::Level_X_Offset);
		Ball_Direction = M_PI - Ball_Direction;
	}

	if (next_y_pos < AsConfig::Border_Y_Offset)
	{
		next_y_pos = AsConfig::Border_Y_Offset - (next_y_pos - AsConfig::Border_Y_Offset);
		Ball_Direction = -Ball_Direction;
	}

	if (next_x_pos > max_x_pos)
	{
		next_x_pos = max_x_pos - (next_x_pos - max_x_pos);
		Ball_Direction = M_PI - Ball_Direction;
	}

	if (next_y_pos > max_y_pos)
	{
		if(level->Has_Floor)
		{
			next_y_pos = max_y_pos - (next_y_pos - max_y_pos);
			Ball_Direction = -Ball_Direction;
		}
		else
		{
			if (next_y_pos > (double)max_y_pos + (double)AsConfig::Ball_Size * 2.0) // Чтобы шарик мог улететь ниже пола, проверяем его max_y_pos ниже видимой границы
				Ball_State = EBS_Lost;
		}
	}

	// Корректируем позицию при отражении от платформы
	if (next_y_pos > platform_y_pos)
	{
		if (next_x_pos >= platform_x_pos && next_x_pos <= platform_x_pos + platform_width)
		{
			next_y_pos = platform_y_pos - (next_y_pos - platform_y_pos);
			Ball_Direction = M_PI + (M_PI - Ball_Direction);
		}
	}

	// Корректируем позицию при отражении от кирпичей
	level->Check_Level_Brick_Hit(next_y_pos, Ball_Direction);

	// Смещаем шарик
	Ball_X_Pos = next_x_pos;
	Ball_Y_Pos = next_y_pos;

	Redraw_Ball();
}
//------------------------------------------------------------------------------------------------------------
EBall_State ABall::Get_State()
{
	return Ball_State; 
}
//------------------------------------------------------------------------------------------------------------
void ABall::Set_State(EBall_State new_state, int x_pos)
{
	switch (new_state)
	{
	case EBS_Normal:
		Ball_X_Pos = (double)x_pos - (double)AsConfig::Ball_Size / 2.0;
		Ball_Y_Pos = Start_Ball_Y_Pos;
		Ball_Speed = 3.0;
		Ball_Direction = M_PI - M_PI_4;
		Redraw_Ball();
		break;


	case EBS_Lost:
		Ball_Speed = 0.0;
		break;


	case EBS_On_Platform:
		Ball_X_Pos = (double)x_pos - (double)AsConfig::Ball_Size / 2.0;
		Ball_Y_Pos = Start_Ball_Y_Pos;
		Ball_Speed = 0.0;
		Ball_Direction = M_PI - M_PI_4;
		Redraw_Ball();
		break;
	
	}
	Ball_State = new_state;
}
//------------------------------------------------------------------------------------------------------------
void ABall::Redraw_Ball()
{
	Ball_Rect.left = (int)Ball_X_Pos * AsConfig::Global_Scale;
	Ball_Rect.top = (int)Ball_Y_Pos * AsConfig::Global_Scale;
	Ball_Rect.right = Ball_Rect.left + AsConfig::Ball_Size * AsConfig::Global_Scale;
	Ball_Rect.bottom = Ball_Rect.top + AsConfig::Ball_Size * AsConfig::Global_Scale;

	InvalidateRect(AsConfig::Hwnd, &Prev_Ball_Rect, FALSE);
	InvalidateRect(AsConfig::Hwnd, &Ball_Rect, FALSE);
}
//------------------------------------------------------------------------------------------------------------