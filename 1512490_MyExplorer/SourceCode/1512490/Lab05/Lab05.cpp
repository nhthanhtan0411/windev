// Lab05.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Lab05.h"
#include <windowsX.h>
#include <winuser.h>
#include <commctrl.h>
#include <windows.h>
#include <commctrl.h>
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "ComCtl32.lib")
//For StrCpy, StrNCat
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <shellapi.h>

#define MAX_LOADSTRING 100
#define NUMBER_OF_INIT_ICON 8 
#define MAX_EXPAND_ICON 3

#define DEFAULT_ICON_INDEX 0

//For init lv column
#define LVCOL_DRIVE_TYPE		0
#define LVCOL_FOLDER_TYPE		1
#define MAX_PATH_LEN 1024


// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
RECT rcClient;                       // The parent window's client area.
HINSTANCE g_Instance;
HWND	g_Wnd;
HWND	g_Tree;
HWND	g_List;
TroGiup* g_Dhelp;

int myComputerIconIndex, desktopIconIndex, driveIconIndex;

HWND TaoCay(long lExtStyle, HWND parentWnd, long ID, HINSTANCE hParentInst, int x, int y, int nWidth, int nHeight, long lStyle);
HWND TaoDS(long lExtStyle, HWND parentWnd, long ID, HINSTANCE hParentInst, int x, int y, int nWidth, int nHeight, long lStyle);
void loadToTree(TroGiup *d, HWND m_hTreeView);
void loadToList(TroGiup *d, HWND m_hListView);
void loadChild(HTREEITEM hCurrSelected, HWND m_hTree);
void loadTreeItemAt(HTREEITEM &hParent, LPCWSTR path, HWND m_hTree);
void loadListItemAt(LPCWSTR path, HWND m_hParent, HWND m_hList, TroGiup *d);
void loadOrExecSelected(HWND m_hList);
void loadDirItemToLisview(HWND m_hParent, HWND m_hList, LPCWSTR path);
void initListColumn(HWND m_hList, int type);
LPCWSTR getPath(HTREEITEM hItem, HWND m_hTree); 
LPWSTR convertTimeStampToString(const FILETIME &ftLastWrite);

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB05, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB05));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB05));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_BTNFACE +1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB05);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; 

   HWND hWnd = CreateWindowEx(0, szWindowClass, L"FILE EXPLORER",
	   WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, 0, 750, 600, NULL, NULL, hInstance, NULL); 

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {

	case WM_CREATE :
	{
		g_Wnd = hWnd;
		g_Dhelp = new TroGiup();
		g_Dhelp->getSystemPc();

		InitCommonControls();

		GetClientRect(hWnd, &rcClient);
		int parentWidth = rcClient.right - rcClient.left;
		int parentHeight = rcClient.bottom - rcClient.top;

		//Create treeview
		long extStyle = 0, style = TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS;
		int x = 0, y = 0;
		int nWidth = parentWidth / 4, nHeight = parentHeight;

		//Tree View
		g_Tree = TaoCay(extStyle, hWnd, IDT_TREEVIEW, g_Instance, x, y, nWidth, nHeight, style);

		loadToTree(g_Dhelp, g_Tree);
		SetFocus(g_Tree);

		//Create listview
		x = nWidth + 1;
		nWidth = (rcClient.right - rcClient.left) * 3 / 4;
		extStyle = WS_EX_CLIENTEDGE;
		style = LVS_REPORT | LVS_ICON | LVS_EDITLABELS | LVS_SHOWSELALWAYS;

		g_List = TaoDS(extStyle, hWnd, IDL_LISTVIEW, g_Instance, x, y, nWidth, nHeight, style);
		loadToList(g_Dhelp, g_List);


	}
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_NOTIFY:
	{
		int nCurSelIndex;

		NMHDR* notifyMess = (NMHDR*)lParam; 
		LPNMTREEVIEW lpnmTree = (LPNMTREEVIEW)notifyMess; 
		HTREEITEM currSelected;

		switch (notifyMess->code)
		{
		case TVN_ITEMEXPANDING: 
			currSelected = lpnmTree->itemNew.hItem;
			loadChild(currSelected, g_Tree);
			break;
		case TVN_SELCHANGED:
			currSelected = TreeView_GetSelection(g_Tree); 
			TreeView_Expand(g_Tree, currSelected, TVE_EXPAND);

			ListView_DeleteAllItems(g_List);
			loadListItemAt(getPath(currSelected, g_Tree), hWnd, g_List, g_Dhelp);
			break;

		case NM_CLICK:
			break;

		case NM_DBLCLK:

			if (notifyMess->hwndFrom == g_List)
				loadOrExecSelected(g_List);
			break;
		}

	}
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

HWND TaoCay(long lExtStyle, HWND parentWnd, long ID, HINSTANCE hParentInst, int x, int y, int nWidth, int nHeight, long lStyle)
{
	//Create
	HWND m_hTree = CreateWindowEx(lExtStyle, WC_TREEVIEW, _T("Tree View"),
		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_SIZEBOX | WS_VSCROLL | WS_TABSTOP | lStyle,
		x, y, nWidth, nHeight, parentWnd,
		(HMENU)ID, hParentInst, NULL);

	HIMAGELIST himl;   
	HBITMAP hbmp;     

	himl = ImageList_Create(16, 16, ILC_COLOR32 | ILC_MASK,
		NUMBER_OF_INIT_ICON, MAX_EXPAND_ICON);

	// Add the open file, closed file, and document bitmaps. 
	hbmp = LoadBitmap(g_Instance, MAKEINTRESOURCE(IDB_DESKTOP));
	desktopIconIndex = ImageList_Add(himl, hbmp, (HBITMAP)NULL);
	DeleteObject(hbmp);

	hbmp = LoadBitmap(g_Instance, MAKEINTRESOURCE(IDB_MYCOMPUTER));
	myComputerIconIndex = ImageList_Add(himl, hbmp, (HBITMAP)NULL);
	DeleteObject(hbmp);

	hbmp = LoadBitmap(g_Instance, MAKEINTRESOURCE(IDB_DRIVE));
	driveIconIndex = ImageList_Add(himl, hbmp, (HBITMAP)NULL);
	DeleteObject(hbmp);
 
	TreeView_SetImageList(m_hTree, himl, TVSIL_NORMAL);

	return m_hTree;
}

HWND TaoDS(long lExtStyle, HWND parentWnd, long ID, HINSTANCE hParentInst, int x, int y, int nWidth, int nHeight, long lStyle)
{
	//Create
	HWND m_hList = CreateWindowEx(lExtStyle, WC_LISTVIEW, _T("List View"),
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_TABSTOP | lStyle,
		x, y, nWidth, nHeight, parentWnd, (HMENU)ID, hParentInst, NULL);

	LVCOLUMN lvCol;
	lvCol.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvCol.fmt = LVCFMT_LEFT;

	lvCol.cx = 150;
	lvCol.pszText = _T("NAME");
	ListView_InsertColumn(m_hList, 0, &lvCol);

	lvCol.fmt = LVCFMT_LEFT;
	lvCol.pszText = _T("TYPE");
	lvCol.cx = 125;
	ListView_InsertColumn(m_hList, 1, &lvCol);

	lvCol.fmt = LVCFMT_LEFT;
	lvCol.cx = 125;
	lvCol.pszText = _T("Total Size");
	ListView_InsertColumn(m_hList, 2, &lvCol);


	lvCol.fmt = LVCFMT_LEFT;
	lvCol.pszText = _T("Free Space");
	lvCol.cx = 125;
	ListView_InsertColumn(m_hList, 3, &lvCol);

	return m_hList;
}

LPCWSTR getPath(HTREEITEM hItem, HWND m_hTree)
{
	TVITEMEX tv;
	tv.mask = TVIF_PARAM;
	tv.hItem = hItem;
	TreeView_GetItem(m_hTree, &tv); 
	return (LPCWSTR)tv.lParam;
}

void loadChild(HTREEITEM hCurrSelected, HWND m_hTree)
{
	HTREEITEM Desktop = TreeView_GetRoot(m_hTree); 
	HTREEITEM myComputer = TreeView_GetChild(m_hTree, Desktop); 
	if (hCurrSelected == myComputer) 
		return;

	HTREEITEM hCurrSelectedChild = TreeView_GetChild(m_hTree, hCurrSelected); 

	if (hCurrSelectedChild != NULL)
	{
		do
		{
			if (TreeView_GetChild(m_hTree, hCurrSelectedChild) == NULL)
			{
				loadTreeItemAt(hCurrSelectedChild, getPath(hCurrSelectedChild, m_hTree), m_hTree);
			}
		} while (hCurrSelectedChild = TreeView_GetNextSibling(m_hTree, hCurrSelectedChild));
	}
	else
	{
		loadTreeItemAt(hCurrSelected, getPath(hCurrSelected, m_hTree), m_hTree);
	}
}

void loadTreeItemAt(HTREEITEM &hParent, LPCWSTR path, HWND m_hTree)
{
	TCHAR dem[MAX_PATH_LEN];
	StrCpy(dem, path); 
	StrCat(dem, _T("\\*")); 
	TV_INSERTSTRUCT tvInsert;
	tvInsert.hParent = hParent;
	tvInsert.hInsertAfter = TVI_SORT;
	tvInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	tvInsert.item.iImage = DEFAULT_ICON_INDEX;
	tvInsert.item.iSelectedImage = DEFAULT_ICON_INDEX;

	WIN32_FIND_DATA ffd;
	HANDLE hFind = FindFirstFileW(dem, &ffd);
	if (hFind == INVALID_HANDLE_VALUE)
		return;

	TCHAR* Fderpath;

	do
	{
		DWORD fileAttribute = ffd.dwFileAttributes;
		if ((fileAttribute & FILE_ATTRIBUTE_DIRECTORY) 
			&& (fileAttribute != FILE_ATTRIBUTE_HIDDEN) 
			&& (_tcscmp(ffd.cFileName, _T(".")) != 0) && (_tcscmp(ffd.cFileName, _T("..")) != 0)) 
		{
			tvInsert.item.pszText = ffd.cFileName;
			Fderpath = new TCHAR[wcslen(path) + wcslen(ffd.cFileName) + 2];

			StrCpy(Fderpath, path);
			if (wcslen(path) != 3)
				StrCat(Fderpath, _T("\\"));
			StrCat(Fderpath, ffd.cFileName);

			tvInsert.item.lParam = (LPARAM)Fderpath;

			HTREEITEM hItem = TreeView_InsertItem(m_hTree, &tvInsert);
		}
	} while (FindNextFileW(hFind, &ffd));

}

void loadToTree(TroGiup *d, HWND m_hTree)
{
	TV_INSERTSTRUCT tv_Insert;

	tv_Insert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;

	tv_Insert.hParent = NULL;
	tv_Insert.hInsertAfter = TVI_ROOT;
	tv_Insert.item.iImage = desktopIconIndex;
	tv_Insert.item.iSelectedImage = desktopIconIndex;
	tv_Insert.item.pszText = _T("Desktop");
	tv_Insert.item.lParam = (LPARAM)_T("Desktop");
	HTREEITEM hDesktop = TreeView_InsertItem(m_hTree, &tv_Insert);

	//Load My Computer
	tv_Insert.hParent = hDesktop;
	tv_Insert.hInsertAfter = TVI_LAST; //Add to last position
	tv_Insert.item.iImage = myComputerIconIndex;
	tv_Insert.item.iSelectedImage = myComputerIconIndex;
	tv_Insert.item.pszText = _T("My Computer");
	tv_Insert.item.lParam = (LPARAM)_T("MyComputer");
	HTREEITEM hMyComputer = TreeView_InsertItem(m_hTree, &tv_Insert);

	for (int i = 0; i < g_Dhelp->getCount(); ++i)
	{
		tv_Insert.hParent = hMyComputer; 
		tv_Insert.item.iImage = driveIconIndex;
		tv_Insert.item.iSelectedImage = driveIconIndex;
		tv_Insert.item.pszText = g_Dhelp->getDisplayName(i); 
		tv_Insert.item.lParam = (LPARAM)g_Dhelp->getLetter(i);
		HTREEITEM hDrive = TreeView_InsertItem(m_hTree, &tv_Insert);

		loadTreeItemAt(hDrive, getPath(hDrive, m_hTree), m_hTree);
	}

	TreeView_Expand(m_hTree, hMyComputer, TVE_EXPAND);
	TreeView_SelectItem(m_hTree, hMyComputer);
}

LPCWSTR getPath(HWND m_hList, int iItem)
{
	LVITEM lv;
	lv.mask = LVIF_PARAM;
	lv.iItem = iItem;
	lv.iSubItem = 0;
	ListView_GetItem(m_hList, &lv);
	return (LPCWSTR)lv.lParam;
}


void loadDirItemToLisview(HWND m_hParent, HWND m_hList, LPCWSTR path)
{
	initListColumn(m_hList, LVCOL_FOLDER_TYPE);
	TCHAR dem[10240];
	StrCpy(dem, path);

	if (wcslen(path) == 3)
		StrCat(dem, _T("*"));
	else
		StrCat(dem, _T("\\*"));

	WIN32_FIND_DATA fd; 
	HANDLE hFind = INVALID_HANDLE_VALUE;
	LV_ITEM lv;
	TCHAR* temporaryPath;
	int itemIndex = 0;

	hFind = FindFirstFileW(dem, &fd);

	if (hFind == INVALID_HANDLE_VALUE)
		return;

	do
	{
		if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) &&
			((fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != FILE_ATTRIBUTE_HIDDEN) &&
			(_tcscmp(fd.cFileName, _T(".")) != 0) && (_tcscmp(fd.cFileName, _T("..")) != 0)) //Ignore . (curr dir) and .. (parent dir)
		{
			temporaryPath = new TCHAR[wcslen(path) + wcslen(fd.cFileName) + 2];
			StrCpy(temporaryPath, path);

			if (wcslen(path) != 3)
				StrCat(temporaryPath, _T("\\"));

			StrCat(temporaryPath, fd.cFileName);

			lv.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
			lv.iItem = itemIndex;
			lv.iSubItem = 0;
			lv.pszText = fd.cFileName;
			lv.iImage = DEFAULT_ICON_INDEX;
			lv.lParam = (LPARAM)temporaryPath;
			ListView_InsertItem(m_hList, &lv);

			ListView_SetItemText(m_hList, itemIndex, 1, convertTimeStampToString(fd.ftLastWriteTime)); //Changes the text of a list - view item or subitem

																										   //Add "File folder" value to Third column
			ListView_SetItemText(m_hList, itemIndex, 2, _T("File folder"));
			itemIndex++;
		}

	} while (FindNextFileW(hFind, &fd));
 
	hFind = FindFirstFileW(dem, &fd);

	if (hFind == INVALID_HANDLE_VALUE)
		return;
	do
	{
		if (((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY) &&
			((fd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) != FILE_ATTRIBUTE_SYSTEM) &&
			((fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != FILE_ATTRIBUTE_HIDDEN))
		{
			temporaryPath = new TCHAR[wcslen(path) + wcslen(fd.cFileName) + 2];
			StrCpy(temporaryPath, path);

			if (wcslen(path) != 3)
				StrCat(temporaryPath, _T("\\"));

			StrCat(temporaryPath, fd.cFileName);
			lv.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
			lv.iItem = itemIndex;
			lv.iSubItem = 0;
			lv.pszText = fd.cFileName;
			lv.lParam = (LPARAM)temporaryPath;

			ListView_InsertItem(m_hList, &lv);
			ListView_SetItemText(m_hList, itemIndex, 1, convertTimeStampToString(fd.ftLastWriteTime));
			DWORD fileSizeLow = fd.nFileSizeLow; 
			ListView_SetItemText(m_hList, itemIndex, 3, KichThuoc::convertByteToStringSize(fileSizeLow));

			itemIndex++;
		}
	} while (FindNextFileW(hFind, &fd));

}

void initListColumn(HWND m_hList, int type)
{
	LVCOLUMN lvCol;
	if (type == LVCOL_DRIVE_TYPE)
	{
		lvCol.mask = LVCF_TEXT | LVCF_FMT;

		lvCol.fmt = LVCFMT_LEFT | LVCF_WIDTH;
		lvCol.cx = 100;
		lvCol.pszText = _T("Type");
		ListView_SetColumn(m_hList, 1, &lvCol);

		lvCol.fmt = LVCFMT_RIGHT | LVCF_WIDTH;
		lvCol.cx = 80;
		lvCol.pszText = _T("Total Size");
		ListView_SetColumn(m_hList, 2, &lvCol);

		lvCol.cx = 80;
		lvCol.pszText = _T("Free Space");
		ListView_SetColumn(m_hList, 3, &lvCol);
	}
	else if (type == LVCOL_FOLDER_TYPE)
	{
		lvCol.mask = LVCF_WIDTH;
		lvCol.cx = 180;
		ListView_SetColumn(m_hList, 0, &lvCol);

		lvCol.mask = LVCF_TEXT | LVCF_FMT;
		lvCol.fmt = LVCFMT_RIGHT;
		lvCol.pszText = _T("Date Modified");
		ListView_SetColumn(m_hList, 1, &lvCol);


		lvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
		lvCol.fmt = LVCFMT_LEFT;
		lvCol.cx = 130;
		lvCol.pszText = _T("Type");
		ListView_SetColumn(m_hList, 2, &lvCol);

		lvCol.pszText = _T("Size");
		ListView_SetColumn(m_hList, 3, &lvCol);
	}
}

void loadOrExecSelected(HWND m_hList)
{
	LPCWSTR FPath = getPath(m_hList,ListView_GetSelectionMark(m_hList));

	WIN32_FIND_DATA fd;
	if (GetFileAttributesEx(FPath, GetFileExInfoStandard, &fd) != 0)
	{
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			ListView_DeleteAllItems(m_hList);
			loadDirItemToLisview(g_Wnd, m_hList, FPath);
		}
		else 
		{
			ShellExecute(NULL, _T("open"), FPath, NULL, NULL, SW_SHOWNORMAL);
		}
	}
}

void loadListItemAt(LPCWSTR path, HWND m_hParent, HWND m_hList, TroGiup *d)
{
	if (path == NULL)
		return;

	LV_ITEM lv;

	if (_tcscmp(path, _T("Desktop")) == 0)
	{
		initListColumn(m_hList, LVCOL_FOLDER_TYPE);

		lv.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
		lv.iItem = 0;
		lv.iSubItem = 0;
		lv.pszText = _T("My Computer");
		lv.iImage = IDI_MYCOMPUTER;
		lv.lParam = (LPARAM)_T("MyComputer");
		ListView_InsertItem(m_hList, &lv); 
	}
	else if (_tcscmp(path, _T("MyComputer")) == 0)
	{
		initListColumn(m_hList, LVCOL_DRIVE_TYPE);

		for (int i = 0; i < d->getCount(); ++i)
		{
			lv.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
			lv.iItem = i;
			lv.iImage = DEFAULT_ICON_INDEX;

			lv.iSubItem = 0;
			lv.pszText = d->getDisplayName(i);
			lv.lParam = (LPARAM)d->getLetter(i);
			ListView_InsertItem(m_hList, &lv);

			lv.mask = LVIF_TEXT;

			lv.iSubItem = 1;
			lv.pszText = d->getType(i);
			ListView_SetItem(m_hList, &lv); 
												
			lv.iSubItem = 2;
			if (wcscmp(d->getType(i), CD_ROM) != 0)
				lv.pszText = d->getTotalSize(i);
			else
				lv.pszText = NULL;

			ListView_SetItem(m_hList, &lv);

			lv.iSubItem = 3;
			if (wcscmp(d->getType(i), CD_ROM) != 0)
				lv.pszText = d->getFreeSpace(i);
			else
				lv.pszText = NULL;

			ListView_SetItem(m_hList, &lv);
		}
	}
	else
		loadDirItemToLisview(g_Wnd, m_hList, path);
}

void loadToList(TroGiup *d, HWND m_hList)
{
	initListColumn(m_hList, LVCOL_DRIVE_TYPE);
	LV_ITEM lv;

	for (int i = 0; i < d->getCount(); ++i)
	{
		lv.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
		lv.iItem = i;
		lv.iImage = DEFAULT_ICON_INDEX;
		lv.iSubItem = 0;
		lv.pszText = d->getDisplayName(i);
		lv.lParam = (LPARAM)d->getLetter(i);
		ListView_InsertItem(m_hList, &lv);

		lv.mask = LVIF_TEXT;

		lv.iSubItem = 1;
		lv.pszText = d->getType(i);
		ListView_SetItem(m_hList, &lv);

		lv.iSubItem = 2;
		if (wcscmp(d->getType(i), CD_ROM) != 0)
			lv.pszText = d->getTotalSize(i);
		else
			lv.pszText = NULL;
		ListView_SetItem(m_hList, &lv);

		lv.iSubItem = 3;
		if (wcscmp(d->getType(i), CD_ROM) != 0)
			lv.pszText = d->getFreeSpace(i);
		else
			lv.pszText = NULL;

		ListView_SetItem(m_hList, &lv);
	}
}
LPWSTR convertTimeStampToString(const FILETIME &ftLastWrite)
{
	TCHAR *dem = new TCHAR[50];
 
	SYSTEMTIME st;

	char szLocalDate[255], szLocalTime[255];

	FileTimeToSystemTime(&ftLastWrite, &st);
	GetDateFormat(LOCALE_USER_DEFAULT, DATE_AUTOLAYOUT, &st, NULL,
		(LPWSTR)szLocalDate, 255);
	GetTimeFormat(LOCALE_USER_DEFAULT, 0, &st, NULL, (LPWSTR)szLocalTime, 255);

	wsprintf(dem, L"%s %s", szLocalDate, szLocalTime);

	return dem;
}