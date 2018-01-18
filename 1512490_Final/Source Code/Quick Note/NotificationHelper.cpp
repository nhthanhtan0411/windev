#include "stdafx.h"
#include "NotificationHelper.h"

BOOL CNotificationHelper::isFirstTimeStartUp()
{
	try {
		HKEY FirstTimeRunKey = nullptr;
		DWORD isFirstTimeRun = 0;

		FirstTimeRunKey = OpenKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Quick Note");
		GetVal(FirstTimeRunKey, L"FirstTimeRun");
		
		if (isFirstTimeRun == 1) {
			return true;
		}
		RegCloseKey(FirstTimeRunKey);
		return false;

	} catch (int e) {
		return false;
	}
}

BOOL CNotificationHelper::setupApplication()
{
	try {
		HKEY FirstTimeRunKey = nullptr;
		DWORD FirstTimeRunValue = 1;

		FirstTimeRunKey = OpenKey(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Quick Note");
		SetVal(FirstTimeRunKey, L"FirstTimeRun", FirstTimeRunValue);

		RegCloseKey(FirstTimeRunKey);
		return true;
	}
	catch (int e) {
		return false;
	}
}


HKEY CNotificationHelper::OpenKey(HKEY hRootKey, wchar_t* strKey)
{
	HKEY hKey;
	LONG nError = RegOpenKeyEx(hRootKey, strKey, NULL, KEY_ALL_ACCESS, &hKey);

	if (nError == ERROR_FILE_NOT_FOUND)
	{
		nError = RegCreateKeyEx(hRootKey, strKey, NULL, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);
	}

	return hKey;
}

void CNotificationHelper::SetVal(HKEY hKey, LPCTSTR lpValue, DWORD data)
{
	LONG nError = RegSetValueEx(hKey, lpValue, NULL, REG_DWORD, (LPBYTE)&data, sizeof(DWORD));
}

DWORD CNotificationHelper::GetVal(HKEY hKey, LPCTSTR lpValue)
{
	DWORD data;		DWORD size = sizeof(data);	DWORD type = REG_DWORD;
	LONG nError = RegQueryValueEx(hKey, lpValue, NULL, &type, (LPBYTE)&data, &size);

	if (nError == ERROR_FILE_NOT_FOUND)
		data = 0; // The value will be created and set to data next time SetVal() is called.

	return data;
}

