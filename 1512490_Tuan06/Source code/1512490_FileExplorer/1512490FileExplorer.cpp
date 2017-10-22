// 1512490FileExplorer.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "1512490FileExplorer.h"

#include <commctrl.h>
#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "ComCtl32.lib")
//For StrCpy, StrNCat
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <shellapi.h>



//For init lv column
#define LVCOL_DRIVE_TYPE		0
#define LVCOL_FOLDER_TYPE		1



#define MAX_LOADSTRING 100
// Global Variables:
HINSTANCE hInst;								
TCHAR szTitle[MAX_LOADSTRING];					
TCHAR szWindowClass[MAX_LOADSTRING];			
RECT rcl;                       
HINSTANCE g_hInstance;
HWND	g_hWnd;
HWND	g_hTreeView;
HWND	g_hListView;
HWND	g_hStatus;
Drive*  g_Drive;
RECT g_TreeViewRect;

int  ThisPCIcon, DriveIcon, FolderIcon;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


void loadExpandedChild(HTREEITEM hCurrSelected, HWND m_hTreeView); 
LPCWSTR getPath(HTREEITEM hItem, HWND m_hTreeView); 
void loadTreeviewItemAt(HTREEITEM &hParent, LPCWSTR path, HWND m_hTreeView);
void loadListviewItemAt(LPCWSTR path, HWND m_hParent, HWND m_hListView, Drive *drive); 
void loadOrExecSelected(HWND m_hListView); 
void loadDirItemToLisview(HWND m_hParent, HWND m_hListView, LPCWSTR path); 
void initListviewColumn(HWND m_hListView, int type); 
LPWSTR GetDateModified(const FILETIME &ftLastWrite);
LPWSTR GetType(const WIN32_FIND_DATA &fd);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDC_1512490FILEEXPLORER, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_1512490FILEEXPLORER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);
	g_hInstance = hInstance;


	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_1512490FILEEXPLORER));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_FOLDER));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_1512490FILEEXPLORER);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_FOLDER));

	return RegisterClassEx(&wcex);
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
	HWND hWnd;

	hInst = hInstance; // Store instance handle in our global variable

	hWnd = CreateWindowEx(0, szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

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
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	static  BOOL        xSizing;

	const int BUFFERSIZE = 260;
	WCHAR buffer1[BUFFERSIZE];
	WCHAR buffer2[BUFFERSIZE];
	WCHAR curPath[BUFFERSIZE];
	WCHAR configPath[BUFFERSIZE];

	// Tạo đường dẫn tuyệt đối tới file config
	GetCurrentDirectory(BUFFERSIZE, curPath);
	wsprintf(configPath, L"%s\\config.ini", curPath);

	switch (message)
	{
	case WM_CREATE:
	{
		g_hWnd = hWnd;
		g_Drive = new Drive();
		g_Drive->getSystemDrives();

		GetClientRect(hWnd, &rcl);
		InitCommonControls();
		// CREATE TREEVIEW
		g_hTreeView = CreateWindowEx(0, WC_TREEVIEW, _T("Tree View"),
			WS_CHILD | WS_VISIBLE | WS_BORDER | WS_SIZEBOX | WS_VSCROLL | WS_TABSTOP | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS,
			CW_USEDEFAULT, 0, rcl.right / 3, rcl.bottom, hWnd,
			(HMENU)IDT_TREEVIEW, g_hInstance, NULL);

		
		
		HIMAGELIST himl;  
		HICON hicon;     
		himl = ImageList_Create(16, 16, ILC_COLOR32 | ILC_MASK, 8, 3);

		hicon = LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_THISPC));
		ThisPCIcon = ImageList_AddIcon(himl, hicon);
		DeleteObject(hicon);

		hicon = LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_FOLDER));
		FolderIcon = ImageList_AddIcon(himl, hicon);
		DeleteObject(hicon);

		hicon = LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_DRIVE));
		DriveIcon = ImageList_AddIcon(himl, hicon);
		DeleteObject(hicon);

		TreeView_SetImageList(g_hTreeView, himl, TVSIL_NORMAL);

		// load file to treeview
		TV_INSERTSTRUCT tvInsert;

		//Let it know we will change the text, icon and lparam associate with item in treeview
		tvInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;

		//Load This PC
		tvInsert.hParent = NULL;
		tvInsert.hInsertAfter = TVI_ROOT;
		tvInsert.item.iImage = ThisPCIcon;
		tvInsert.item.iSelectedImage = ThisPCIcon;
		tvInsert.item.pszText = _T("This PC");
		tvInsert.item.lParam = (LPARAM)_T("This PC");
		HTREEITEM hThisPC = TreeView_InsertItem(g_hTreeView, &tvInsert);

		//Load volume
		for (int i = 0; i < g_Drive->getCount(); ++i)
		{
			tvInsert.hParent = hThisPC; //Add as children of My Computer
			tvInsert.item.iImage = DriveIcon;
			tvInsert.item.iSelectedImage = DriveIcon;
			tvInsert.item.pszText = g_Drive->getDisplayName(i); //Get volume label
			tvInsert.item.lParam = (LPARAM)g_Drive->getDriveLetter(i);
			HTREEITEM hDrive = TreeView_InsertItem(g_hTreeView, &tvInsert);

			loadTreeviewItemAt(hDrive, getPath(hDrive, g_hTreeView), g_hTreeView);
		}

		//Expand and select This PC
		TreeView_Expand(g_hTreeView, hThisPC, TVE_EXPAND);
		TreeView_SelectItem(g_hTreeView, hThisPC);

		SetFocus(g_hTreeView);

		//CREATE LISTVIEW
		g_hListView = CreateWindowEx(WS_EX_CLIENTEDGE,
			WC_LISTVIEW,                // list view class
			_T("List View"),                         // no default text
			WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT |
			LVS_EDITLABELS | WS_EX_CLIENTEDGE| WS_TABSTOP | LVS_ICON | LVS_EDITLABELS | LVS_SHOWSELALWAYS,
			rcl.right/3, 0,
			(rcl.right - rcl.left) * 2 / 3 + 1, rcl.bottom,
			hWnd,
			(HMENU)IDL_LISTVIEW,
			hInst,
			NULL);



		
		//Init 5 columns
		LVCOLUMN lvCol;

		//Let the LVCOLUMN know that we will set the format, header text and width of it
		lvCol.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
		lvCol.fmt = LVCFMT_LEFT;

		lvCol.cx = 150;
		lvCol.pszText = _T("Name");
		ListView_InsertColumn(g_hListView, 0, &lvCol);

		lvCol.fmt = LVCFMT_LEFT;
		lvCol.pszText = _T("Type");
		lvCol.cx = 125;
		ListView_InsertColumn(g_hListView, 1, &lvCol);

		lvCol.fmt = LVCFMT_LEFT;
		lvCol.cx = 125;
		lvCol.pszText = _T("Total Size");
		ListView_InsertColumn(g_hListView, 2, &lvCol);


		lvCol.fmt = LVCFMT_LEFT;
		lvCol.pszText = _T("Free Space");
		lvCol.cx = 125;
		ListView_InsertColumn(g_hListView, 3, &lvCol);

		//load file to list view

		//Init column of Listview
		initListviewColumn(g_hListView, LVCOL_DRIVE_TYPE);
		LV_ITEM lv;

		for (int i = 0; i < g_Drive->getCount(); ++i)
		{
			//Let ListView know that we'r going to change item text, image and param
			lv.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;

			//Load Label name and default icon to first column
			lv.iItem = i;
			lv.iImage = FolderIcon;
			lv.iSubItem = 0;
			lv.pszText = g_Drive->getDisplayName(i);
			lv.lParam = (LPARAM)g_Drive->getDriveLetter(i);
			ListView_InsertItem(g_hListView, &lv);

			//
			lv.mask = LVIF_TEXT;

			//Load Type of directory to second column
			lv.iSubItem = 1;
			lv.pszText = g_Drive->getDriveType(i);
			ListView_SetItem(g_hListView, &lv);

			//Load total size to third column
			lv.iSubItem = 2;
			if (wcscmp(g_Drive->getDriveType(i), CD_ROM) != 0)
				lv.pszText = g_Drive->getTotalSize(i);
			else
				lv.pszText = NULL;
			ListView_SetItem(g_hListView, &lv);

			//Load Free Space to last column
			lv.iSubItem = 3;
			if (wcscmp(g_Drive->getDriveType(i), CD_ROM) != 0)
				lv.pszText = g_Drive->getFreeSpace(i);
			else
				lv.pszText = NULL;

			//Set
			ListView_SetItem(g_hListView, &lv);
		}

		// STATUS BAR
		g_hStatus = CreateWindow(STATUSCLASSNAME, NULL, WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP | SBARS_TOOLTIPS, 0, 0, CW_USEDEFAULT, 100,
			hWnd, (HMENU)IDC_STATUSBAR, hInst, NULL);
		int nStatusSize[] = { rcl.right , -1 };
		SendMessage(g_hStatus, SB_SETPARTS, 3, (LPARAM)&nStatusSize);
		MoveWindow(g_hStatus, 0, 0, rcl.right, rcl.bottom, TRUE);

		TCHAR *buffer = new TCHAR[34];
		wsprintf(buffer, _T("This PC có tổng cộng %d ổ đĩa"), g_Drive->getCount());

		
	}

	break;
	case WM_COMMAND:
		RECT rect;
		int width;
		int height;
		int w, h;
		if (GetWindowRect(hWnd, &rect))
		{
			width = rect.right - rect.left;
			height = rect.bottom - rect.top;
		}
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_FILE_UPDATESIZE:
			TCHAR buf1[32];
			_itow_s(width, buf1, 10);
			TCHAR buf2[32];
			_itow_s(height, buf2, 10);
			WritePrivateProfileString(L"section", L"width", buf1, configPath);
			WritePrivateProfileString(L"section", L"height", buf2, configPath);
			MessageBox(0, L"Update successfully!", L"Success", MB_OK);
			break;
		case ID_FILE_LOADSIZE:
			GetPrivateProfileString(L"section", L"width", L"Default value", buffer1, BUFFERSIZE, configPath);
			GetPrivateProfileString(L"section", L"height", L"Default value", buffer2, BUFFERSIZE, configPath);
			w = _wtoi(buffer1);
			h = _wtoi(buffer2);
			MoveWindow(hWnd, rect.left, rect.top, w, h, TRUE);
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_NOTIFY:
	{
		int nCurSelIndex;

		//The program has started and loaded all necessary component

		NMHDR* notifyMess = (NMHDR*)lParam; //Notification Message
		LPNMTREEVIEW lpnmTree = (LPNMTREEVIEW)notifyMess; //Contains information about a tree-view notification message
		HTREEITEM currSelected;

		switch (notifyMess->code)
		{
		case TVN_ITEMEXPANDING: //This event fire when user expand or colapse item in Tree View
								//Load child item of current selected child if they were not loaded before
			currSelected = lpnmTree->itemNew.hItem;
			loadExpandedChild(currSelected, g_hTreeView);
			break;
		case TVN_SELCHANGED:
			//Retrieve currently selected item in TreeView
			currSelected = TreeView_GetSelection(g_hTreeView); //You can explicitly get by TreeView_GetNextItem with TVGN_CARET flag
			TreeView_Expand(g_hTreeView, currSelected, TVE_EXPAND);

			ListView_DeleteAllItems(g_hListView); //Clear ListView
			loadListviewItemAt(getPath(currSelected, g_hTreeView), hWnd, g_hListView, g_Drive);
			break;

		case NM_CLICK:
			if (notifyMess->hwndFrom == g_hListView)
			{
				int nCurSelIndex = ListView_GetNextItem(GetDlgItem(hWnd, IDL_LISTVIEW), -1, LVNI_FOCUSED);
				TCHAR *text = new TCHAR[256];
				LVITEM lv;
				lv.mask = LVIF_TEXT;
				lv.iItem = nCurSelIndex;
				lv.iSubItem = 0;
				lv.pszText = text;
				lv.cchTextMax = 256;


				lv.iSubItem = 2;
				ListView_GetItem(g_hListView, &lv);

				if (!StrCmpI(lv.pszText, _T("Thư mục")))
					SendMessage(GetDlgItem(hWnd, IDC_STATUSBAR), SB_SETTEXT, 0, NULL);
				else
				{
					lv.iSubItem = 3;
					ListView_GetItem(g_hListView, &lv);
					SendMessage(GetDlgItem(hWnd, IDC_STATUSBAR), SB_SETTEXT, 0, (LPARAM)text);
				}

			}
			
			break;

		case NM_DBLCLK:
			//Get hwndFrom for window handle to the control sending the message
			//To check whether this event fire by Listview
			if (notifyMess->hwndFrom == g_hListView)
			{
				loadOrExecSelected(g_hListView);
				
			}
				
			break;
		case NM_CUSTOMDRAW: //Ve lai cua so con
			RECT newTreeRC;
			GetClientRect(g_hTreeView, &newTreeRC);

			if (newTreeRC.right != g_TreeViewRect.right)
			{
				RECT tree;
				GetWindowRect(GetDlgItem(hWnd, IDT_TREEVIEW), &tree);

				RECT main;
				GetWindowRect(hWnd, &main);

				MoveWindow(g_hListView, tree.right - tree.left, 0, main.right - tree.right, tree.bottom - tree.top, TRUE);
				ListView_Arrange(g_hListView, LVA_ALIGNTOP);
				g_TreeViewRect = newTreeRC;
			}
			break;
		}

	}
	break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_SIZE:

		RECT treeRC;
		GetWindowRect(g_hTreeView, &treeRC);
		MoveWindow(g_hTreeView, 0, 0, treeRC.right - treeRC.left, 443, SWP_SHOWWINDOW);
		GetWindowRect(g_hTreeView, &g_TreeViewRect);

		RECT tree;
		GetWindowRect(GetDlgItem(hWnd, IDT_TREEVIEW), &tree);

		RECT main;
		GetWindowRect(hWnd, &main);

		MoveWindow(g_hListView, tree.right - tree.left, 0, main.right - tree.right, tree.bottom - tree.top, TRUE);
		ListView_Arrange(g_hListView, LVA_ALIGNTOP);

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

LPCWSTR getPath(HTREEITEM hItem, HWND m_hTreeView)
{
	TVITEMEX tv; //Specifies or receives attributes of a tree-view item.
	tv.mask = TVIF_PARAM;
	tv.hItem = hItem;
	TreeView_GetItem(m_hTreeView, &tv); //Retrieves some or all of a tree-view item's attributes. 
	return (LPCWSTR)tv.lParam;
}

LPCWSTR getPath(HWND m_hListView, int iItem)
{
	LVITEM lv;
	lv.mask = LVIF_PARAM;
	lv.iItem = iItem;
	lv.iSubItem = 0;
	ListView_GetItem(m_hListView, &lv);
	return (LPCWSTR)lv.lParam;
}

void loadExpandedChild(HTREEITEM hCurrSelected, HWND m_hTreeView)
{
	HTREEITEM thisPC = TreeView_GetRoot(m_hTreeView); //Return the top most or very first item of the TreeView
	HTREEITEM hCurrSelectedChild = TreeView_GetChild(m_hTreeView, hCurrSelected); //Get the first child of treeview item

	if (hCurrSelectedChild != NULL)
	{
		do
		{
			//Get child of this Current selected child, if result is NULL -> It never be loaded
			if (TreeView_GetChild(m_hTreeView, hCurrSelectedChild) == NULL)
			{
				//Load all child of Current Selected Child	
				loadTreeviewItemAt(hCurrSelectedChild, getPath(hCurrSelectedChild, m_hTreeView), m_hTreeView);
			}
		} while (hCurrSelectedChild = TreeView_GetNextSibling(m_hTreeView, hCurrSelectedChild));
	}
	else
	{
		//Possitively will not happen :D
		loadTreeviewItemAt(hCurrSelected, getPath(hCurrSelected, m_hTreeView), m_hTreeView);
	}
}

void loadTreeviewItemAt(HTREEITEM &hParent, LPCWSTR path, HWND m_hTreeView)
{
	TCHAR buffer[10240];
	StrCpy(buffer, path); //Copy the path of item (include drive letter path)
	StrCat(buffer, _T("\\*")); //Add to find all item in directory

							   //Insert new item to TreeView
	TV_INSERTSTRUCT tvInsert;
	tvInsert.hParent = hParent;
	tvInsert.hInsertAfter = TVI_SORT;
	tvInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
	tvInsert.item.iImage = FolderIcon;
	tvInsert.item.iSelectedImage = FolderIcon;

	WIN32_FIND_DATA ffd; //Contains information about the file that is found by Find first file and Find next file
	HANDLE hFind = FindFirstFileW(buffer, &ffd);

	//If the function fails or fails to locate files from the search string
	if (hFind == INVALID_HANDLE_VALUE)
		return;

	TCHAR* folderPath;

	do
	{
		DWORD fileAttribute = ffd.dwFileAttributes;
		if ((fileAttribute & FILE_ATTRIBUTE_DIRECTORY) //Get only directory and folder
			&& (fileAttribute != FILE_ATTRIBUTE_HIDDEN) //Not hidden
			&& (_tcscmp(ffd.cFileName, _T(".")) != 0) && (_tcscmp(ffd.cFileName, _T("..")) != 0)) //Ignore . (curr dir) and .. (parent dir)
		{
			//Set file name
			tvInsert.item.pszText = ffd.cFileName;
			folderPath = new TCHAR[wcslen(path) + wcslen(ffd.cFileName) + 2];

			//Set path
			StrCpy(folderPath, path);
			if (wcslen(path) != 3)
				StrCat(folderPath, _T("\\"));
			StrCat(folderPath, ffd.cFileName);

			tvInsert.item.lParam = (LPARAM)folderPath;

			HTREEITEM hItem = TreeView_InsertItem(m_hTreeView, &tvInsert);
		}
	} while (FindNextFileW(hFind, &ffd));

}

void loadListviewItemAt(LPCWSTR path, HWND m_hParent, HWND m_hListView, Drive *drive)
{
	if (path == NULL)
		return;
	loadDirItemToLisview(g_hWnd, m_hListView, path);
}


void loadOrExecSelected(HWND m_hListView)
{
	LPCWSTR filePath = getPath(m_hListView, ListView_GetSelectionMark(m_hListView));

	WIN32_FIND_DATA fd;

	//Retrieves attributes for a specified file or directory.
	if (GetFileAttributesEx(filePath, GetFileExInfoStandard, &fd) != 0)
	{
		//Check whether it's folder or directory
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			//Delete and reload item in Listview
			ListView_DeleteAllItems(m_hListView);
			loadDirItemToLisview(g_hWnd, m_hListView, filePath);
		}
		else
		{
			ShellExecute(NULL, _T("open"), filePath, NULL, NULL, SW_SHOWNORMAL);
		}
	}
}



void loadDirItemToLisview(HWND m_hParent, HWND m_hListView, LPCWSTR path)
{
	initListviewColumn(m_hListView, LVCOL_FOLDER_TYPE);
	TCHAR buffer[10240];

	//Copy path to buffer
	StrCpy(buffer, path);

	if (wcslen(path) == 3)
		StrCat(buffer, _T("*"));
	else
		StrCat(buffer, _T("\\*"));

	//Variables
	WIN32_FIND_DATA fd; //Contains information about the file that is found by the FindFirstFile or FindNextFile function
	HANDLE hFind = INVALID_HANDLE_VALUE;
	LV_ITEM lv;
	TCHAR* temporaryPath;
	int itemIndex = 0;



	hFind = FindFirstFileW(buffer, &fd);


	if (hFind == INVALID_HANDLE_VALUE)
		return;

	DWORD fileSizeCount = 0;
	hFind = FindFirstFileW(buffer, &fd);

	if (hFind == INVALID_HANDLE_VALUE)
		return;

	do
	{
		if (((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY) &&
			((fd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM) != FILE_ATTRIBUTE_SYSTEM) &&
			((fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != FILE_ATTRIBUTE_HIDDEN))
		{
			//Get file path
			temporaryPath = new TCHAR[wcslen(path) + wcslen(fd.cFileName) + 2];
			StrCpy(temporaryPath, path);

			if (wcslen(path) != 3)
				StrCat(temporaryPath, _T("\\"));

			StrCat(temporaryPath, fd.cFileName);

			//Add name and path to first column
			lv.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
			lv.iItem = itemIndex;
			lv.iSubItem = 0;
			lv.pszText = fd.cFileName;
			lv.lParam = (LPARAM)temporaryPath;
			ListView_InsertItem(m_hListView, &lv);


			ListView_SetItemText(m_hListView, itemIndex, 1, GetDateModified(fd.ftLastWriteTime));
			//Last column is Size
			ListView_SetItemText(m_hListView, itemIndex, 2, GetType(fd));
			
			DWORD fileSizeLow = fd.nFileSizeLow; //The low-order DWORD value of the file size, in bytes
			ListView_SetItemText(m_hListView, itemIndex, 3, CDrive::convertByteToStringSize(fileSizeLow));
			fileSizeCount += fd.nFileSizeLow;
			itemIndex++;
		}
	} while (FindNextFileW(hFind, &fd));

	
	DWORD fileSizeLow = fd.nFileSizeLow;
	SendMessage(GetDlgItem(m_hParent, IDC_STATUSBAR), SB_SETTEXT, 0, (LPARAM)CDrive::convertByteToStringSize(fileSizeCount));
}

void initListviewColumn(HWND m_hListView, int type)
{
	LVCOLUMN lvCol;
	if (type == LVCOL_DRIVE_TYPE)
	{
		lvCol.mask = LVCF_TEXT | LVCF_FMT;

		lvCol.fmt = LVCFMT_LEFT | LVCF_WIDTH;
		lvCol.cx = 100;
		lvCol.pszText = _T("Type");
		ListView_SetColumn(m_hListView, 1, &lvCol);

		lvCol.fmt = LVCFMT_RIGHT | LVCF_WIDTH;
		lvCol.cx = 80;
		lvCol.pszText = _T("Total Size");
		ListView_SetColumn(m_hListView, 2, &lvCol);

		lvCol.cx = 80;
		lvCol.pszText = _T("Free Space");
		ListView_SetColumn(m_hListView, 3, &lvCol);
	}
	else if (type == LVCOL_FOLDER_TYPE)
	{
		lvCol.mask = LVCF_WIDTH;
		lvCol.cx = 180;
		ListView_SetColumn(m_hListView, 0, &lvCol);

		lvCol.mask = LVCF_TEXT | LVCF_FMT;
		lvCol.fmt = LVCFMT_RIGHT;
		lvCol.pszText = _T("Date Modified");
		ListView_SetColumn(m_hListView, 1, &lvCol);


		lvCol.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;
		lvCol.fmt = LVCFMT_LEFT;
		lvCol.cx = 130;
		lvCol.pszText = _T("Type");
		ListView_SetColumn(m_hListView, 2, &lvCol);

		lvCol.pszText = _T("Size");
		ListView_SetColumn(m_hListView, 3, &lvCol);
	}
}

LPWSTR GetDateModified(const FILETIME &ftLastWrite)
{
	//Chuyển đổi sang local time
	SYSTEMTIME stUTC, stLocal;
	FileTimeToSystemTime(&ftLastWrite, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

	TCHAR *buffer = new TCHAR[50];
	wsprintf(buffer, _T("%02d/%02d/%04d %02d:%02d %s"),
		stLocal.wDay, stLocal.wMonth, stLocal.wYear,
		(stLocal.wHour>12) ? (stLocal.wHour / 12) : (stLocal.wHour),
		stLocal.wMinute,
		(stLocal.wHour>12) ? (_T("PM")) : (_T("AM")));

	return buffer;
}

#define ENTIRE_STRING NULL

LPWSTR GetType(const WIN32_FIND_DATA &fd)
{
	int nDotPos = StrRStrI(fd.cFileName, ENTIRE_STRING, _T(".")) - fd.cFileName;
	int len = wcslen(fd.cFileName);

	if (nDotPos < 0 || nDotPos >= len) //Nếu không tìm thấy
		return _T("Unkown");

	TCHAR *szExtension = new TCHAR[len - nDotPos + 1];
	int i;

	for (i = nDotPos; i < len; ++i)
		szExtension[i - nDotPos] = fd.cFileName[i];
	szExtension[i - nDotPos] = NULL; //Kí tự kết thúc chuỗi

	if (!StrCmpI(szExtension, _T(".htm")) || !StrCmpI(szExtension, _T(".html")))
	{
		return _T("Web page");
	}
	TCHAR pszOut[256];
	HKEY hKey;
	DWORD dwType = REG_SZ;
	DWORD dwSize = 256;

	//Kiếm handle của extension tương ứng trong registry
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, szExtension, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("Unkown");
	}

	if (RegQueryValueEx(hKey, NULL, NULL, &dwType, (PBYTE)pszOut, &dwSize) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("Unkown");
	}
	RegCloseKey(hKey);

	//Kiếm mô tả về thông tin của extension thông qua handle của key tương ứng trong registry
	TCHAR *pszPath = new TCHAR[1000];
	dwSize = 1000;
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, pszOut, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("Unkown");
	}

	if (RegQueryValueEx(hKey, NULL, NULL, &dwType, (PBYTE)pszPath, &dwSize) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return _T("Unkown");
	}
	RegCloseKey(hKey);

	return pszPath;
}

