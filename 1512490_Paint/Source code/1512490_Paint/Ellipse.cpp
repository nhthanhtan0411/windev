#include "stdafx.h"
#include "Ellipse.h"


CEllipse::CEllipse()
{
}

void CEllipse::Draw(HDC hdc) {
	gHpen = CreatePen(0, sSize, sColor);
	SelectObject(hdc, gHpen);
	Ellipse(hdc, a.x, a.y, b.x, b.y);
	DeleteObject(gHpen);
}

CShape* CEllipse::Create() { return new CEllipse; }

void CEllipse::SetData(int ax, int ay, int bx, int by, COLORREF clr, int sz) {
	a.x = ax;
	a.y = ay;
	b.x = bx;
	b.y = by;
	sColor = clr;
	sSize = sz;
}


CEllipse::~CEllipse()
{
}
