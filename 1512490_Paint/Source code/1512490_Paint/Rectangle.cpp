#include "stdafx.h"
#include "Rectangle.h"

CRectangle::CRectangle()
{
}

void CRectangle::Draw(HDC hdc)
{
	gHpen = CreatePen(0, sSize, sColor);
	SelectObject(hdc, gHpen);
	Rectangle(hdc, a.x, a.y, b.x, b.y);
	DeleteObject(gHpen);
}

CShape* CRectangle::Create()
{
	return new CRectangle;
}

void CRectangle::SetData(int ax, int ay, int bx, int by, COLORREF clr, int sz)
{
	a.x = ax;
	a.y = ay;
	b.x = bx;
	b.y = by;
	sColor = clr;
	sSize = sz;
}


CRectangle::~CRectangle()
{
}
