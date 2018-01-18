#include "stdafx.h"
#include "PiePart.h"


CPiePart::CPiePart()
{
	mX = 600;
	mY = 10;
	dwRadius = 80;
	xStartAngle = 0;
	xSweepAngle = 0;
}

CPiePart::CPiePart(int x, int y)
{
	mX = x;
	mY = y;
	dwRadius = 80;
	xStartAngle = 0;
	xSweepAngle = 0;
}


CPiePart::~CPiePart()
{
}

int CPiePart::getX()
{
	return mX;
}

int CPiePart::getY()
{
	return mY;
}

DWORD CPiePart::getR()
{
	return dwRadius;
}

float CPiePart::getStartAngle()
{
	return xStartAngle;
}

float CPiePart::getSweepAngle()
{
	return xSweepAngle;
}

int CPiePart::setX(int x)
{
	mX = x;
	return mX;
}

int CPiePart::setY(int y)
{
	mY = y;
	return mY;
}

DWORD CPiePart::setR(DWORD newR)
{
	dwRadius = newR;
	return dwRadius;
}

float CPiePart::setStartAngle(float stAng)
{
	xStartAngle = stAng;
	return xStartAngle;
}

float CPiePart::setSweepAngle(float swAng)
{
	xSweepAngle = swAng;
	return xSweepAngle;
}
