#pragma once
class CDrive
{
private:
	__int64 mTotalSize;
	__int64 mFreeSpace;


public:
	CDrive();
	CDrive(__int64 totalSize, __int64 freeSpace);
	~CDrive();

	static LPWSTR convertByteToStringSize(__int64 nSize);

	LPWSTR getTotalSize();
	LPWSTR getFreeSpace();

};

