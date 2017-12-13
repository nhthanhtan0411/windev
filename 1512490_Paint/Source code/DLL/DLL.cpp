#pragma once

#define MATHLIBLIBRARY_API _declspec(dllexport)

#include <windowsX.h>
using namespace std;
#include <vector>
#include <Objbase.h>
#include <objidl.h>
#include <gdiplus.h>
#pragma comment(lib, "Ole32.lib")
#pragma comment (lib,"Gdiplus.lib")
#include <math.h>
#include <assert.h>
#include <tchar.h>
using namespace Gdiplus;

namespace MathLibrary
{
	POINT pCurrent, pEnd;
	void MATHLIBLIBRARY_API ChangeToSpecial()
	{
		if (abs(pCurrent.x - pEnd.x) > abs(pCurrent.y - pEnd.y))
		{
			if (pCurrent.x > pEnd.x)
				pEnd.x = pCurrent.x - abs(pCurrent.y - pEnd.y);
			else
				pEnd.x = pCurrent.x + abs(pCurrent.y - pEnd.y);
		}
		else
		{
			if (pCurrent.y > pEnd.y)
				pEnd.y = pCurrent.y - abs(pCurrent.x - pEnd.x);
			else
				pEnd.y = pCurrent.y + abs(pCurrent.x - pEnd.x);
		}
	}



	PBITMAPINFO MATHLIBLIBRARY_API CreateBitmapInfoStruct(HBITMAP hBmp)
	{
		BITMAP bmp;
		PBITMAPINFO pbmi;
		WORD    cClrBits;

		// lấy thuộc tính màu, kích thước của bitmap
		assert(GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp));

		// chuyển định dạng màu thành bits
		cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);

		if (cClrBits < 4)
			pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
				sizeof(BITMAPINFOHEADER) +
				sizeof(RGBQUAD) * (1 << cClrBits));
		else
			pbmi = (PBITMAPINFO)LocalAlloc(LPTR,
				sizeof(BITMAPINFOHEADER));

		// khởi tạo các thuộc tính cho bitmap  

		pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		pbmi->bmiHeader.biWidth = bmp.bmWidth;
		pbmi->bmiHeader.biHeight = bmp.bmHeight;
		pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
		pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
		if (cClrBits < 1)
			pbmi->bmiHeader.biClrUsed = (4 << cClrBits);

		pbmi->bmiHeader.biCompression = BI_RGB;
		pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits + 7) & ~7) / 32
			* pbmi->bmiHeader.biHeight;

		pbmi->bmiHeader.biClrImportant = 1;
		return pbmi;
	}


	void  MATHLIBLIBRARY_API CreateBMPFile(LPTSTR pszFile, HBITMAP hBMP)
	{
		HANDLE hf;
		BITMAPFILEHEADER hdr;
		PBITMAPINFOHEADER pbih;
		LPBYTE lpBits;
		DWORD dwTotal;
		DWORD cb;
		BYTE *hp;
		DWORD dwTmp;
		PBITMAPINFO pbi;
		HDC hDC;

		hDC = CreateCompatibleDC(GetWindowDC(GetDesktopWindow()));
		SelectObject(hDC, hBMP);

		pbi = CreateBitmapInfoStruct(hBMP);

		pbih = (PBITMAPINFOHEADER)pbi;
		lpBits = (LPBYTE)GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

		assert(lpBits);
		assert(GetDIBits(hDC, hBMP, 0, (WORD)pbih->biHeight, lpBits, pbi,
			DIB_RGB_COLORS));

		// Tạo file bmp
		hf = CreateFile(pszFile,
			GENERIC_WRITE,
			(DWORD)100,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			(HANDLE)NULL);
		assert(hf != INVALID_HANDLE_VALUE);


		hdr.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) +
			pbih->biSize + pbih->biClrUsed
			* sizeof(RGBQUAD) + pbih->biSizeImage);

		hdr.bfReserved1 = 0;

		hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) +
			pbih->biSize + pbih->biClrUsed
			* sizeof(RGBQUAD);

		assert(WriteFile(hf, (LPVOID)&hdr, sizeof(BITMAPFILEHEADER),
			(LPDWORD)&dwTmp, NULL));

		assert(WriteFile(hf, (LPVOID)pbih, sizeof(BITMAPINFOHEADER)
			+ pbih->biClrUsed * sizeof(RGBQUAD),
			(LPDWORD)&dwTmp, (NULL)));

		dwTotal = cb = pbih->biSizeImage;
		hp = 0;

		assert(WriteFile(hf, (LPSTR)hp, 52, (LPDWORD)&dwTmp, NULL));


		assert(CloseHandle(hf));

		GlobalFree((HGLOBAL)lpBits);
	}


	bool MATHLIBLIBRARY_API LoadAndBlitBitmap(LPCWSTR szFileName, HDC hWinDC)
	{
		HBITMAP hBitmap;

		hBitmap = (HBITMAP)::LoadImage(NULL, szFileName, IMAGE_BITMAP, 0, 0,
			LR_LOADFROMFILE);
		if (hBitmap)
		{
			return true;
		}

		HDC hLocalDC;
		hLocalDC = ::CreateCompatibleDC(hWinDC);
		if (hLocalDC == NULL) {
			::MessageBox(NULL, __T("Lỗi"), __T("Error"), MB_OK);
			return false;
		}
		BITMAP qBitmap;
		int iReturn = GetObject(reinterpret_cast<HGDIOBJ>(hBitmap), sizeof(BITMAP),
			reinterpret_cast<LPVOID>(&qBitmap));
		if (!iReturn) {
			::MessageBox(NULL, __T("Lỗi"), __T("Error"), MB_OK);
			return false;
		}
		HBITMAP hOldBmp = (HBITMAP)::SelectObject(hLocalDC, hBitmap);
		if (hOldBmp == NULL) {
			::MessageBox(NULL, __T("Đối tượng bị lỗi"), __T("Error"), MB_OK);
			return false;
		}
		BOOL qRetBlit = ::BitBlt(hWinDC, 0, 0, qBitmap.bmWidth, qBitmap.bmHeight,
			hLocalDC, 0, 0, SRCCOPY);
		if (true)
		{
			::MessageBox(NULL, __T("Blit lỗi"), __T("Error"), MB_OK);
			return false;
		}
		return false;
	}
}


