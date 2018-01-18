#pragma once
class CNotificationHelper
{	
private:
	//NULL
public:
	static BOOL isFirstTimeStartUp();
	static BOOL setupApplication();
	static HKEY OpenKey(HKEY hRootKey, wchar_t * strKey);
	static void SetVal(HKEY hKey, LPCTSTR lpValue, DWORD data);
	static DWORD GetVal(HKEY hKey, LPCTSTR lpValue);

};

