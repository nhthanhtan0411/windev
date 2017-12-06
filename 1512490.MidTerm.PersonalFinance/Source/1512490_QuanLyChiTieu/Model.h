#pragma once
#include <string>
class Model
{
public:
	WCHAR mType[20];
	unsigned long long mMoney;
	std::wstring mDescription;
	Model();
	~Model();
};

