#pragma once
#include "Shape.h"
class CEllipse : public CShape
{
public:
	CEllipse();
	void Draw(HDC hdc);
	CShape* Create();
	void SetData(int ax, int ay, int bx, int by, COLORREF clr, int sz);
	~CEllipse();
};

