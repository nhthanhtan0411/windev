#pragma once
#include "1512490_Paint.h"

class CShape
{
protected:
	POINT a, b;
	COLORREF sColor;
	int sSize;
public:
	virtual void Draw(HDC hdc) = 0;
	virtual CShape* Create() = 0;
	virtual void SetData(int a, int b, int c, int d, COLORREF clr, int sz) = 0;
	CShape();
	~CShape();
};

