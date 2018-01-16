// ShortcutKeyDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <windowsx.h>

#define IDC_EDIT_TAG                    1000
#define IDC_EDIT_DATA                   1001

#define EXPORT __declspec(dllexport)

HHOOK _hHook = NULL;
HINSTANCE _hInst;
HWND handle;

// hook
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) 
{
	if (nCode < 0)
	{
		return CallNextHookEx(_hHook, nCode, wParam, lParam);
	}

	if (wParam == WM_KEYDOWN)
	{
		KBDLLHOOKSTRUCT *mHookData = (KBDLLHOOKSTRUCT *)lParam;

		if (mHookData->vkCode == VK_SPACE && GetAsyncKeyState(VK_LWIN))
		{
			// if show -> hide
			if (IsWindowVisible(handle))
			{
				ShowWindow(handle, SW_HIDE);
			}
			// if hide -> show
			else
			{
				HWND hTag = GetDlgItem(handle, IDC_EDIT_TAG);
				HWND hData = GetDlgItem(handle, IDC_EDIT_DATA);

				SetWindowText(hTag, L"");
				SetWindowText(hData, L"");

				// make 'handle' on top 
				SetWindowPos(handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

				ShowWindow(handle, SW_SHOW);

				SetFocus(hTag);
			}

			return TRUE;
		}

		//// window + space
		//if (mHookData->vkCode == VK_SPACE && GetAsyncKeyState(VK_LWIN)) {

		//	
		//}
	}

	// 
	return CallNextHookEx(_hHook, nCode, wParam, lParam);
}

EXPORT bool _doInstallHook(HWND hWnd)
{
	// if already initialized
	if (_hHook != NULL)
	{
		return true;
	}

	// init
	_hHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)KeyboardProc, _hInst, 0);

	// check result 
	if (_hHook)
	{
		MessageBox(0,L"OK", 0, 0);
		handle = hWnd;
		return true;
	}
	else
	{
		MessageBox(0, L"Fail", 0, 0);
		return false;
	}
}

EXPORT void _doRemoveHook(HWND hWnd) 
{
	// check before cancel to avoid errors
	if (_hHook == NULL)
	{
		return;
	}

	// release
	UnhookWindowsHookEx(_hHook);
	_hHook = NULL;
}

