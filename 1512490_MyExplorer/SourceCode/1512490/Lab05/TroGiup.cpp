#include "stdafx.h"
#include "TroGiup.h"

#include <shellapi.h>
//Dùng để sử dụng hàm StrCpy, StrNCat
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")


TroGiup::TroGiup()
{
	mVanTu = NULL;
	mNhanAmLuong = NULL;
	mLoai = NULL;
	mNumber = 0;
}

TroGiup::~TroGiup()
{
	for (int i = 0; i < mNumber; ++i)
	{
		delete[] mVanTu[i];
		delete[] mNhanAmLuong[i];
		delete[] mLoai[i];
	}

	delete[] mVanTu;
	delete[] mNhanAmLuong;
	delete[] mLoai;
	mNumber = 0;
}

TCHAR* TroGiup::getType(int position)
{
	return mLoai[position];
}

TCHAR* TroGiup::getLetter(const int &i)
{
	return mVanTu[i];
}


TCHAR* TroGiup::getDisplayName(const int &i)
{
	return mNhanAmLuong[i];
}

int TroGiup::getCount()
{
	return mNumber;
}

void TroGiup::getSystemPc()
{
	TCHAR buffer[MAX_BUFFER_LEN];
	TCHAR* volumeTempName = new TCHAR[VOLUME_NAME_SIZE];

	
	GetLogicalDriveStrings(MAX_BUFFER_LEN, buffer);

	mNumber = 0;
	for (int i = 0; (buffer[i] != '\0') || (buffer[i - 1] != '\0'); i++)
	{
		if (buffer[i] == '\0')
		{
			mNumber++;
		}
	}
	mVanTu = new TCHAR*[mNumber];
	mNhanAmLuong = new TCHAR*[mNumber];
	mLoai = new TCHAR*[mNumber];
	mKichThuoc = new KichThuoc*[mNumber];

	for (int i = 0; i < mNumber; ++i)
	{
		mVanTu[i] = new TCHAR[MAX_DRIVE_LETTER];
		mNhanAmLuong[i] = new TCHAR[MAX_DISPLAY_NAME];
		mLoai[i] = new TCHAR[MAX_TYPE_LEN];
	}

	int j, k;
	int index = 0;

	for (j = 0; j < mNumber; ++j)
	{
		k = 0;
		while (buffer[index] != 0)
		{
			mVanTu[j][k++] = buffer[index++];
		}
		mVanTu[j][k] = '\0'; 
		++index;
	}

	int nType;
	for (int i = 0; i < mNumber; ++i)
	{
		nType = GetDriveType(mVanTu[i]);
		switch (nType)
		{
		case DRIVE_FIXED:
			StrCpy(mLoai[i], FIXED_DRIVE);
			break;
		case DRIVE_REMOVABLE:
			StrCpy(mLoai[i], REMOVABLE_DRIVE);
			break;
		case DRIVE_REMOTE:
			StrCpy(mLoai[i], REMOTE_DRIVE);
			break;
		case DRIVE_CDROM:
			StrCpy(mLoai[i], CD_ROM);
			break;
		default:
			break;
		}
		mKichThuoc[i] = getSize(i);
		StrCpy(buffer, _T(""));
		if ((nType == DRIVE_FIXED) || ((nType == DRIVE_REMOVABLE)) || (nType == DRIVE_REMOTE))
		{
			GetVolumeInformation(mVanTu[i], buffer, MAX_BUFFER_LEN, VOLUME_SERIAL_NUMBER, MAX_COMPONENT_LEN, FILE_SYSTEM_FLAG, FILE_SYSTEM_NAME_BUFF, FILE_SYSTEM_NAME_SIZE);
			StrCpy(volumeTempName, buffer);
		}
		else
			if (nType == DRIVE_CDROM)
			{
				GetVolumeInformation(mVanTu[i], buffer, MAX_BUFFER_LEN, VOLUME_SERIAL_NUMBER, MAX_COMPONENT_LEN, FILE_SYSTEM_FLAG, FILE_SYSTEM_NAME_BUFF, FILE_SYSTEM_NAME_SIZE);

				if (wcslen(buffer) == 0)
				{
					StrCpy(volumeTempName, _T("CD-ROM"));
				}
				else
					StrCpy(volumeTempName, buffer);
			}
			else if (((i == 0) || (i == 1)) && (nType == DRIVE_REMOVABLE)) //i == 0 or i == 1 because loppy disk usually is A:\\ drive
			{
				StrCpy(volumeTempName, _T("We don't talk to floppy disk any more"));
			}
		if (wcslen(volumeTempName) == 0)
		{
			StrCpy(mNhanAmLuong[i], _T("Local Disk"));
		}
		else
		{
			StrCpy(mNhanAmLuong[i], volumeTempName);
		}
		StrCat(mNhanAmLuong[i], _T(" ("));
		StrNCat(mNhanAmLuong[i], mVanTu[i], 3);
		StrCat(mNhanAmLuong[i], _T(")"));
	}
}

KichThuoc* TroGiup::getSize(int i)
{
	__int64 totalSize;
	__int64 freeSpace;

	SHGetDiskFreeSpaceEx(getLetter(i), NULL, (PULARGE_INTEGER)&totalSize, (PULARGE_INTEGER)&freeSpace);

	KichThuoc* driveSize = new KichThuoc(totalSize, freeSpace);

	return driveSize;
}

LPWSTR TroGiup::getFreeSpace(int i)
{
	return mKichThuoc[i]->getFreeSpace();
}

LPWSTR TroGiup::getTotalSize(int i)
{
	return mKichThuoc[i]->getTotalSize();
}