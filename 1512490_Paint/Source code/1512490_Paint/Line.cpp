#include "stdafx.h"
#include "Line.h"

CLine::CLine()
{
}

void CLine::Draw(HDC hdc)
{
	gHpen = CreatePen(0, sSize, sColor);
	SelectObject(hdc, gHpen);
	MoveToEx(hdc, a.x, a.y, NULL);
	LineTo(hdc, b.x, b.y);
	DeleteObject(gHpen);
}

CShape* CLine::Create()
{
	return new CLine;
}

void CLine::SetData(int ax, int ay, int bx, int by, COLORREF clr, int sz)
{
	a.x = ax;
	a.y = ay;
	b.x = bx;
	b.y = by;
	sColor = clr;
	sSize = sz;
}

CLine::~CLine()
{
}
