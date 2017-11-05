#pragma once
#include "Shape.h"
class CRectangle : public CShape
{
public:
	CRectangle();
	void Draw(HDC hdc);
	CShape* Create();
	void SetData(int ax, int ay, int bx, int by, COLORREF clr, int sz);
	~CRectangle();
};

