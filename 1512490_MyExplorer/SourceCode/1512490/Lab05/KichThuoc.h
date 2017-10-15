#pragma once
class KichThuoc
{
private:
	__int64 mTotalSize;
	__int64 mFreeSpace;


public:
	KichThuoc();
	KichThuoc(__int64 totalSize, __int64 freeSpace);
	~KichThuoc();

	static LPWSTR convertByteToStringSize(__int64 nSize);

	LPWSTR getTotalSize();
	LPWSTR getFreeSpace();

};