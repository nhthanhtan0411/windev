#pragma once
class CPiePart
{
public:
	CPiePart();
	CPiePart(int x, int y);
	~CPiePart();
	int getX();
	int getY();
	DWORD getR();
	float getStartAngle();
	float getSweepAngle();
	int setX(int x);
	int setY(int y);
	DWORD setR(DWORD newR);
	float setStartAngle(float stAng);
	float setSweepAngle(float swAng);
private:
	int mX;
	int mY;
	DWORD dwRadius;
	float xStartAngle;
	float xSweepAngle;
};

