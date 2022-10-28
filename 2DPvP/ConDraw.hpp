#include <algorithm>
#include <conio.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <string>

using std::swap;

#ifndef CONDRAW_HPP_
#define CONDRAW_HPP_

#include "WinAPI.hpp"

struct region
{				   // 区域
	pos s, f;	   // s: 左上, f: 右下
	int HighLight; // 高亮状态
	int LowColor, UpColor;
	char Text[255];
};
/* Function Name: SetRegion                              *
 * Use:  Set A Region                                    *
 * Call: Func(region, sx, sy, fx, fy, LowColor, UpColor) *
 * Vars:                                                 *
 *   region:   需要初始化的区域                            *
 *   sx/sy:    区域左上角的坐标                            *
 *   fx/fy:    区域右下角的坐标                            *
 *   LowColor: 普通状态区域颜色                            *
 *   UpColor:  高亮状态区域颜色                            *
 * (Make By ShiYihang)                                   */
void SetRegion(region &Td, int sx, int sy, int fx, int fy, int ColorL, int ColorU, int TextColorL, int TextColorU, const char *Str)
{
	Td.HighLight = 0;
	Td.s.x = sx, Td.f.x = fx;
	Td.s.y = sy, Td.f.y = fy;
	Td.LowColor = CON_COLOR(ColorL, TextColorL);
	Td.UpColor = CON_COLOR(ColorU, TextColorU);
	strcpy(Td.Text, Str);
}

/* Function Name: DisplayRegion *
 * Use: Display A Region        *
 * (Make By ShiYihang)          */
void DisplayRegion(region &Td)
{
	if (Td.s.y > Td.f.y)
		swap(Td.s, Td.f);
	if (Td.s.x > Td.f.x)
		swap(Td.s, Td.f);
	SetColor(Td.LowColor);
	if (Td.HighLight)
		SetColor(Td.UpColor);
	for (int i = Td.s.x; i <= Td.f.x; ++i) {
		for (int j = Td.s.y; j <= Td.f.y; ++j)
			SetCursor(i, j) /*, putch(' ')*/, std::cout << "  " << std::endl;
		SetCursor(i, Td.f.y + 1) /*, putch(' ')*/, std::cout << " " << std::endl;
	}
	SetCursor((Td.s.x + Td.f.x) >> 1, Td.s.y + 1);
	std ::cout << Td.Text << std ::endl;
}

/* Function Name: CheckCursorOn               *
 * Use: If Cursor in the Region then Return 1 *
 * (Make By ShiYihang)                        */
int CheckCursorOn(region &Td, pos CursorPoint)
{
	if (Td.s.y > Td.f.y)
		swap(Td.s, Td.f);
	if (Td.s.x > Td.f.x)
		swap(Td.s, Td.f);
	return (CursorPoint.x >= Td.s.x && CursorPoint.x <= Td.f.x && CursorPoint.y >= Td.s.y && CursorPoint.y <= Td.f.y);
}

#endif
