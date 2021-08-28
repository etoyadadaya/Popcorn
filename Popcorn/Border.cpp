#include "Border.h"


//AsBorder
//------------------------------------------------------------------------------------------------------------
AsBorder::AsBorder()
:	Border_Blue_Pen(0), Border_White_Pen(0), Border_Blue_Brush(0), Border_White_Brush(0)
{
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Init()
{
	AsConfig::Create_Pen_Brush(85, 255, 255, Border_Blue_Pen, Border_Blue_Brush);
	AsConfig::Create_Pen_Brush(255, 255, 255, Border_White_Pen, Border_White_Brush);
}
//------------------------------------------------------------------------------------------------------------	
void AsBorder::Draw(HDC hdc, RECT &paint_areah)
{// Рисует рамку уровня

	int i;

	// 1. Линия слева
	for (i = 0; i < 50; i++)
		Draw_Element(hdc, 2, 1 + i * 4, false);

	// 2. Линия справа
	for (i = 0; i < 50; i++)
		Draw_Element(hdc, 201, 1 + i * 4, false);

	// 3. Линия сверху
	for (i = 0; i < 50; i++)
		Draw_Element(hdc, 3 + i * 4, 0, true);
}
//------------------------------------------------------------------------------------------------------------
bool AsBorder::Check_Hit(double next_x_pos, double next_y_pos, ABall *ball)
{
	bool got_hit = false;

	// Корректируем позицию при отражении от рамки
	if (next_x_pos - ball->Radius < AsConfig::Border_X_Offset)
	{
		got_hit = true;
		ball->Ball_Direction = M_PI - ball->Ball_Direction;
	}

	if (next_y_pos - ball->Radius < AsConfig::Border_Y_Offset)
	{
		got_hit = true;
		ball->Ball_Direction = -ball->Ball_Direction;
	}

	if (next_x_pos + ball->Radius > AsConfig::Max_X_Pos)
	{
		got_hit = true;
		ball->Ball_Direction = M_PI - ball->Ball_Direction;
	}

	if (next_y_pos + ball->Radius > AsConfig::Max_Y_Pos)
	{
		if(AsConfig::Level_Has_Floor)
		{
			got_hit = true;
			ball->Ball_Direction = -ball->Ball_Direction;
		}
		else
		{
			if (next_y_pos + ball->Radius > (double)AsConfig::Max_Y_Pos + ball->Radius * 4.0) // Чтобы шарик мог улететь ниже пола, проверяем его max_y_pos ниже видимой границы
				ball->Set_State(EBS_Lost, next_x_pos);
		}
	}

	return got_hit;
}
//------------------------------------------------------------------------------------------------------------
void AsBorder::Draw_Element(HDC hdc, int x, int y, bool top_border)
{// Рисует элемент рамки уровня

 // Основная линия
	SelectObject(hdc, Border_Blue_Pen);
	SelectObject(hdc, Border_Blue_Brush);

	if (top_border)
		Rectangle(hdc, x *	AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + 4) * AsConfig::Global_Scale, (y + 4) * AsConfig::Global_Scale);
	else
		Rectangle(hdc, (x + 1) * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + 4) * AsConfig::Global_Scale, (y + 4) * AsConfig::Global_Scale);

	// Белая кайма
	SelectObject(hdc, Border_White_Pen);
	SelectObject(hdc, Border_White_Brush);

	if (top_border)
		Rectangle(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + 4) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale);
	else
		Rectangle(hdc, x * AsConfig::Global_Scale, y * AsConfig::Global_Scale, (x + 1) * AsConfig::Global_Scale, (y + 4) * AsConfig::Global_Scale);

	// Перфорация
	SelectObject(hdc, AsConfig::BG_Pen);
	SelectObject(hdc, AsConfig::BG_Brush);

	if (top_border)
		Rectangle(hdc, (x + 2) * AsConfig::Global_Scale, (y + 2) * AsConfig::Global_Scale, (x + 3) * AsConfig::Global_Scale, (y + 3) * AsConfig::Global_Scale);
	else
		Rectangle(hdc, (x + 2) * AsConfig::Global_Scale, (y + 1) * AsConfig::Global_Scale, (x + 3) * AsConfig::Global_Scale, (y + 2) * AsConfig::Global_Scale);
}
//------------------------------------------------------------------------------------------------------------
