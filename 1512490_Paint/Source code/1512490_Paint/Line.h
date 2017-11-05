#pragma once
#include "Shape.h"

class CLine : public CShape
{
public:
	CLine();
	void Draw(HDC hdc);
	CShape* Create();
	void SetData(int ax, int ay, int bx, int by, COLORREF clr, int sz);
	~CLine();
};

