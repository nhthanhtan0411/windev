#pragma once
#include <string>
class MoHinh
{
public:
	WCHAR mType[20];
	unsigned long long mMoney;
	std::wstring mDescription;
	MoHinh();
	~MoHinh();
};

