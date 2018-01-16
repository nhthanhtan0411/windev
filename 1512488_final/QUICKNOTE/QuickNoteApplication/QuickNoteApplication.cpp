// 1412228_QuickNote.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "QuickNoteApplication.h"

#pragma comment(linker,"\"/manifestdependency:type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment(lib, "ComCtl32.lib")

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_QUICKNOTE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_QUICKNOTE));

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

	return (int)msg.wParam;
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

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NOTE));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	//wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	//wcex.hbrBackground = (HBRUSH)(COLOR_BTNSHADOW);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_QUICKNOTE);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_NOTE));

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
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
		CW_USEDEFAULT, 0, 700, 500, nullptr, nullptr, hInstance, nullptr);

	// add noti
	nidApp.uVersion = NOTIFYICON_VERSION_4;
	nidApp.cbSize = sizeof(NOTIFYICONDATA);
	nidApp.hWnd = (HWND)hWnd;
	nidApp.uID = IDI_NOTE;
	nidApp.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	nidApp.hIcon = LoadIcon(hInstance, (LPCTSTR)MAKEINTRESOURCE(IDI_NOTE));
	nidApp.uCallbackMessage = WM_USER_SHELLICON;
	LoadString(hInstance, NULL, nidApp.szTip, MAX_LOADSTRING);
	Shell_NotifyIcon(NIM_ADD, &nidApp);

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
	case WM_CLOSE:
	{
		ShowWindow(hWnd, SW_HIDE);
		return 0;
	}

	// when minimize window
	case WM_SIZE:
		if (wParam == SIZE_MINIMIZED)
		{
			ShowWindow(hWnd, SW_HIDE);
			return 0;
		}
		break;

		// noti area
	case WM_USER_SHELLICON:
		// mouse event
		if (LOWORD(lParam) == WM_RBUTTONDOWN)
		{

			POINT lpClickPoint;
			UINT uFlag = MF_BYPOSITION | MF_STRING;
			GetCursorPos(&lpClickPoint);
			HMENU hPopMenu = CreatePopupMenu();

			InsertMenu(hPopMenu, 0xFFFFFFFF, uFlag, ID_NOTI_VIEWNOTE, _T("View Notes"));
			InsertMenu(hPopMenu, 0xFFFFFFFF, uFlag, ID_NOTI_STATISTICS, _T("View Statitistics"));
			InsertMenu(hPopMenu, 0xFFFFFFFF, MF_BYPOSITION | MF_STRING, IDM_EXIT, _T("Exit"));

			SetForegroundWindow(hWnd);
			TrackPopupMenu(hPopMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_BOTTOMALIGN, lpClickPoint.x, lpClickPoint.y, 0, hWnd, NULL);
			return TRUE;
		}
		else if (LOWORD(lParam) == WM_LBUTTONDOWN)
		{
			ShowWindow(hWnd, SW_RESTORE);
			isStatistics = true;
			SendMessage(hWnd, WM_COMMAND, IDC_BTN_STATISTICS, NULL);
		}
		break;

		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
		HANDLE_MSG(hWnd, WM_NOTIFY, OnNotify);

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

// Message handler for note box.
INT_PTR CALLBACK Note(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	HWND hTag		= GetDlgItem(hDlg, IDC_EDIT_TAG);
	HWND hTagCombo	= GetDlgItem(hDlg, IDC_COMBO_TAG);
	HWND hTagAuto	= GetDlgItem(hDlg, IDC_TEXT_TAG);
	HWND hData		= GetDlgItem(hDlg, IDC_EDIT_DATA);

	switch (message)
	{
	case WM_INITDIALOG:
	{
		oldEditProc = (WNDPROC)SetWindowLongPtr(hTag, GWLP_WNDPROC, (LONG_PTR)subEditProc);
		prepareNoteDialog();

		SetFocus(hTag);

		return (INT_PTR)TRUE;
	}

	case WM_COMMAND:

		// tag text change
		if ((HIWORD(wParam) == EN_CHANGE) &&
			(LOWORD(wParam) == IDC_EDIT_TAG))
		{
			int size = GetWindowTextLength(hTag);
			WCHAR *bufferTag = new WCHAR[size + 1];
			GetWindowText(hTag, bufferTag, size + 1);

			wstring a = myTags.autoTag(wstring(bufferTag));
			vector<wchar_t> buf(a.begin(), a.end());
			buf.push_back(0);
			SetWindowText(hTagAuto, buf.data());
		}

		// for close dialog
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}

		if (LOWORD(wParam) == IDC_DIALOG_CANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}

		if (LOWORD(wParam) == IDC_DIALOG_ADD)
		{
			if (!isEditNote)
			{
				// add new node
				int size = GetWindowTextLength(hTag);
				WCHAR *bufferTag = new WCHAR[size + 1];
				GetWindowText(hTag, bufferTag, size + 1);

				size = GetWindowTextLength(hData);
				WCHAR *bufferData = new WCHAR[size + 1];
				GetWindowText(hData, bufferData, size + 1);

				WCHAR *name = new WCHAR[100];

				// get time now
				time_t t = time(0);
				struct tm * now = localtime(&t);

				wsprintf(name, L"%02d%02d%04d %02d%02d%02d", now->tm_mday, now->tm_mon + 1, (now->tm_year + 1900), now->tm_hour, now->tm_min, now->tm_sec);

				myNotes.addNote(wstring(name), wstring(bufferTag), wstring(bufferData));
				myTags.addNote(myNotes.size(), wstring(bufferTag));
			}
			else
			{
				// edit note
				int size = GetWindowTextLength(hTag);
				WCHAR *bufferTag = new WCHAR[size + 1];
				GetWindowText(hTag, bufferTag, size + 1);

				size = GetWindowTextLength(hData);
				WCHAR *bufferData = new WCHAR[size + 1];
				GetWindowText(hData, bufferData, size + 1);

				int selectedNote = getSelectedNote();

				myNotes.editNote(myNotes.getNoteID(selectedNote), wstring(bufferTag), wstring(bufferData));
				myTags.editNote(selectedNote + 1, wstring(bufferTag));
			}

			EndDialog(hDlg, LOWORD(wParam));
			updateTreeTag();
			updateListNote();
			return (INT_PTR)TRUE;
		}
		break;
	}

	return (INT_PTR)FALSE;
}

// Message handler for rename box.
INT_PTR CALLBACK Rename(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	HWND hOld = GetDlgItem(hDlg, IDC_EDIT_OLDNAME);
	HWND hNew = GetDlgItem(hDlg, IDC_EDIT_NEWNAME);

	int selectedTagPosition = -1;

	switch (message)
	{
	case WM_INITDIALOG:
	{
		selectedTagPosition = getSelectedTag() - 2;

		// convert wstring to LPWSTR
		wstring a = myTags.getName(selectedTagPosition);
		vector<wchar_t> buf(a.begin(), a.end());
		buf.push_back(0);
		SetWindowText(hOld, buf.data());

		SetFocus(hNew);

		return (INT_PTR)TRUE;
	}
	case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}

		if (LOWORD(wParam) == IDOK)
		{

			int size = GetWindowTextLength(hNew);
			WCHAR *buffer = new WCHAR[size + 1];
			GetWindowText(hNew, buffer, size + 1);

			wstring result = wstring(buffer);

			bool remove = true;
			for (int j = 0; j < result.length(); j++)
			{
				if (result[j] != L' ')
				{
					remove = false;
					break;
				}
			}

			if (remove)
			{
				// convert wstring to LPWSTR
				wstring a = TAG_INVALID;
				vector<wchar_t> buf_1(a.begin(), a.end());
				buf_1.push_back(0);

				// convert wstring to LPWSTR
				wstring b = NOTIFICATION;
				vector<wchar_t> buf_2(b.begin(), b.end());
				buf_2.push_back(0);

				MessageBox(hDlg, a.data(), b.data(), MB_OK | MB_ICONERROR);
				return (INT_PTR)TRUE;
			}

			// clear ' '
			while (result[0] == L' ')
			{
				result.erase(0, 1);
			}

			while (result[result.length() - 1] == L' ')
			{
				result.erase(result.length() - 1, 1);
			}

			// name exist
			if (myTags.isExistTagName(result))
			{
				// convert wstring to LPWSTR
				wstring a = TAG_EXIST;
				vector<wchar_t> buf_1(a.begin(), a.end());
				buf_1.push_back(0);

				// convert wstring to LPWSTR
				wstring b = NOTIFICATION;
				vector<wchar_t> buf_2(b.begin(), b.end());
				buf_2.push_back(0);

				MessageBox(hDlg, a.data(), b.data(), MB_OK | MB_ICONERROR);
				return (INT_PTR)TRUE;
			}

			selectedTagPosition = getSelectedTag() - 2;

			myTags.renameTag(selectedTagPosition, result);

			EndDialog(hDlg, LOWORD(wParam));
			updateTreeTag();
		}
	}

	return (INT_PTR)FALSE;
}

// ----------------------- ... -----------------------
BOOL OnCreate(HWND hWnd, LPCREATESTRUCT lpCreateStruct)
{
	ShowWindow(hWnd, SW_HIDE);

	INITCOMMONCONTROLSEX icc;
	icc.dwSize = sizeof(icc);
	icc.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icc);

	// create a folder to save ScreenShot
	CreateDirectory(L".\\DATA_Notes", NULL);

	// get system font
	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	HFONT hFont = CreateFont(lf.lfHeight, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, VIETNAMESE_CHARSET,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);

	// ------------------------------ Treeview ------------------------------
	treeTag = CreateWindowEx(0, WC_TREEVIEW, _T("Tree View"),
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_TABSTOP | WS_BORDER
		| TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS,
		20, 35, 250, 350, hWnd,
		(HMENU)IDC_TREE_TAG, NULL, NULL);

	// creating image list and put it into the tree control
	HIMAGELIST hImageList;      // Image list array hadle
	HBITMAP hBitMap;            // bitmap handler

								// load the picture from the resource, attach the image, to the image list
	hImageList = ImageList_Create(16, 16, ILC_COLOR16, 1, 10);
	hBitMap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TAG_1));
	ImageList_Add(hImageList, hBitMap, NULL);
	hBitMap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_TAG_2));
	ImageList_Add(hImageList, hBitMap, NULL);
	hBitMap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_FOLDER_1));
	ImageList_Add(hImageList, hBitMap, NULL);
	hBitMap = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_FOLDER_2));
	ImageList_Add(hImageList, hBitMap, NULL);

	// put it onto the tree control
	DeleteObject(hBitMap);
	SendDlgItemMessage(hWnd, IDC_TREE_TAG, TVM_SETIMAGELIST, 0, (LPARAM)hImageList);

	// ------------------------------ Listview ------------------------------
	listNote = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, _T("List View"),
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_TABSTOP | LVS_EDITLABELS | LVS_SHOWSELALWAYS,
		290, 35, 380, 350, hWnd, (HMENU)IDC_LIST_NOTE, NULL, NULL);

	// set LVS_REPORT as default
	SetWindowLong(listNote, GWL_STYLE, GetWindowLong(listNote, GWL_STYLE) & ~(LVS_REPORT) | LVS_REPORT);

	// create a column
	LVCOLUMN lvCol;
	lvCol.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH;
	lvCol.fmt = LVCFMT_CENTER;
	lvCol.cx = 296;
	lvCol.pszText = _T("NOTES");
	ListView_InsertColumn(listNote, 0, &lvCol);

	// ------------------------------ Button ------------------------------
	txtNoteCount = CreateWindowEx(0, L"STATIC", TOTAL_NOTE.c_str(), WS_CHILD | WS_VISIBLE | SS_RIGHT, 300, 10, 150, 20, hWnd, NULL, hInst, NULL);

	groupTag = CreateWindowEx(0, L"BUTTON", TAG.c_str(), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 20, 385, 250, 80, hWnd, NULL, hInst, NULL);

	btnTagRename = CreateWindowEx(0, L"BUTTON", RENAME.c_str(), WS_CHILD | WS_VISIBLE, 40, 415, 100, 30, hWnd, (HMENU)IDC_BTN_TAG_RENAME, hInst, NULL);
	btnTagDelete = CreateWindowEx(0, L"BUTTON", _DELETE.c_str(), WS_CHILD | WS_VISIBLE, 155, 415, 100, 30, hWnd, (HMENU)IDC_BTN_TAG_DELETE, hInst, NULL);

	groupNote = CreateWindowEx(0, L"BUTTON", NOTE.c_str(), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 290, 385, 380, 80, hWnd, NULL, hInst, NULL);

	btnNoteAdd = CreateWindowEx(0, L"BUTTON", ADD.c_str(), WS_CHILD | WS_VISIBLE, 320, 415, 100, 30, hWnd, (HMENU)IDC_BTN_NOTE_ADD, hInst, NULL);
	btnNoteEdit = CreateWindowEx(0, L"BUTTON", EDIT.c_str(), WS_CHILD | WS_VISIBLE, 435, 415, 100, 30, hWnd, (HMENU)IDC_BTN_NOTE_EDIT, hInst, NULL);
	btnNoteDelete = CreateWindowEx(0, L"BUTTON", _DELETE.c_str(), WS_CHILD | WS_VISIBLE, 550, 415, 100, 30, hWnd, (HMENU)IDC_BTN_NOTE_DELETE, hInst, NULL);

	btnStatistics = CreateWindowEx(0, L"BUTTON", STATISTICS.c_str(), WS_CHILD | WS_VISIBLE, 560, 5, 100, 30, hWnd, (HMENU)IDC_BTN_STATISTICS, hInst, NULL);

	// set font 
	SendMessage(btnTagRename, WM_SETFONT, WPARAM(hFont), TRUE);
	SendMessage(btnTagDelete, WM_SETFONT, WPARAM(hFont), TRUE);
	SendMessage(btnNoteAdd, WM_SETFONT, WPARAM(hFont), TRUE);
	SendMessage(btnNoteEdit, WM_SETFONT, WPARAM(hFont), TRUE);
	SendMessage(btnNoteDelete, WM_SETFONT, WPARAM(hFont), TRUE);
	SendMessage(btnStatistics, WM_SETFONT, WPARAM(hFont), TRUE);
	SendMessage(groupTag, WM_SETFONT, WPARAM(hFont), TRUE);
	SendMessage(groupNote, WM_SETFONT, WPARAM(hFont), TRUE);


	// ------------------------------ Data ------------------------------
	myTags.readData();
	myNotes.readData();

	updateTreeTag();
	updateListNote();

	// create dialog add note
	dialogAddNote = CreateDialog(hInst, MAKEINTRESOURCE(IDD_NOTEBOX), hWnd, Note);

	// connect hook
	doInstallHook(dialogAddNote);

	return true;
}

LRESULT OnNotify(HWND hWnd, int idFrom, NMHDR *pnm) {
	switch (pnm->code)
	{
	case TVN_ITEMEXPANDING:

		break;

	case TVN_SELCHANGED:
	{
		int selectedTag = getSelectedTag();

		if (selectedTag == 0)
		{
			updateListNote();
		}
		else if (selectedTag == 1)
		{
			ListView_DeleteAllItems(listNote);
			wstring a = TOTAL_NOTE + to_wstring(0);
			vector<wchar_t> buf(a.begin(), a.end());
			buf.push_back(0);
			SetWindowText(txtNoteCount, buf.data());
		}
		else
		{
			updateListNote(selectedTag - 2);
		}

		break;
	}
	case NM_DBLCLK:
		if (getSelectedNote() == -1)
		{
			return 0;
		}

		isEditNote = true;
		prepareNoteDialog();
		ShowWindow(dialogAddNote, SW_SHOW);
		break;
	}

	return 0;
}

void OnCommand(HWND hWnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
	case ID_NOTI_VIEWNOTE:
		ShowWindow(hWnd, SW_RESTORE);
		isStatistics = true;
		SendMessage(hWnd, WM_COMMAND, IDC_BTN_STATISTICS, NULL);
		break;

	case ID_NOTI_STATISTICS:
		ShowWindow(hWnd, SW_RESTORE);
		isStatistics = false;
		SendMessage(hWnd, WM_COMMAND, IDC_BTN_STATISTICS, NULL);
		break;

	case IDM_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		break;

	case IDM_EXIT:
		DestroyWindow(hWnd);
		break;

	case IDC_BTN_TAG_DELETE:
	{
		int tagPostion = getSelectedTag() - 2;
		if (tagPostion <= -1)
		{
			// convert wstring to LPWSTR
			wstring a = SELECT_TAG_TO_DELETE;
			vector<wchar_t> buf_1(a.begin(), a.end());
			buf_1.push_back(0);

			// convert wstring to LPWSTR
			wstring b = NOTIFICATION;
			vector<wchar_t> buf_2(b.begin(), b.end());
			buf_2.push_back(0);

			MessageBox(hWnd, a.data(), b.data(), MB_OK | MB_ICONWARNING);

			return;
		}

		// check tag's note count == 0
		if (!myTags.isDelete(tagPostion))
		{
			// convert wstring to LPWSTR
			wstring a = TAG_DELETE_ERROR;
			vector<wchar_t> buf_1(a.begin(), a.end());
			buf_1.push_back(0);

			// convert wstring to LPWSTR
			wstring b = NOTIFICATION;
			vector<wchar_t> buf_2(b.begin(), b.end());
			buf_2.push_back(0);

			MessageBox(hWnd, a.data(), b.data(), MB_OK | MB_ICONERROR);

			return;
		}

		// convert wstring to LPWSTR
		wstring a = TAG_DELETE_CONFIRM;
		vector<wchar_t> buf_1(a.begin(), a.end());
		buf_1.push_back(0);

		// convert wstring to LPWSTR
		wstring b = NOTIFICATION;
		vector<wchar_t> buf_2(b.begin(), b.end());
		buf_2.push_back(0);

		if (MessageBox(hWnd, a.data(), b.data(), MB_OKCANCEL | MB_ICONWARNING) == IDOK)
		{
			myTags.deleteTag(tagPostion);
		}

		updateTreeTag();
		updateListNote();

		break;
	}
	case IDC_BTN_TAG_RENAME:
		if (getSelectedTag() <= 1)
		{
			// convert wstring to LPWSTR
			wstring a = SELECT_TAG_TO_RENAME;
			vector<wchar_t> buf_1(a.begin(), a.end());
			buf_1.push_back(0);

			// convert wstring to LPWSTR
			wstring b = NOTIFICATION;
			vector<wchar_t> buf_2(b.begin(), b.end());
			buf_2.push_back(0);

			MessageBox(hWnd, a.data(), b.data(), MB_OK | MB_ICONWARNING);

			return;
		}
		DialogBox(hInst, MAKEINTRESOURCE(IDD_RENAMEBOX), hWnd, Rename);
		break;

	case IDC_BTN_NOTE_ADD:
		isEditNote = false;
		prepareNoteDialog();
		ShowWindow(dialogAddNote, SW_SHOW);
		break;

	case IDC_BTN_NOTE_DELETE:
	{
		// convert wstring to LPWSTR
		wstring a = NOTE_DELETE_CONFIRM;
		vector<wchar_t> buf_1(a.begin(), a.end());
		buf_1.push_back(0);

		// convert wstring to LPWSTR
		wstring b = NOTIFICATION;
		vector<wchar_t> buf_2(b.begin(), b.end());
		buf_2.push_back(0);

		if (MessageBox(hWnd, a.data(), b.data(), MB_OKCANCEL | MB_ICONWARNING) == IDOK)
		{
			int selectedNote = getSelectedNote();

			myNotes.deleteNote(myNotes.getNoteID(selectedNote));
			myTags.deleteNote(selectedNote + 1);
		}

		updateTreeTag();
		updateListNote();
		break;
	}
	case IDC_BTN_NOTE_EDIT:
		if (getSelectedNote() == -1)
		{
			// convert wstring to LPWSTR
			wstring a = SELECT_NOTE_TO_EDIT;
			vector<wchar_t> buf_1(a.begin(), a.end());
			buf_1.push_back(0);

			// convert wstring to LPWSTR
			wstring b = NOTIFICATION;
			vector<wchar_t> buf_2(b.begin(), b.end());
			buf_2.push_back(0);

			MessageBox(hWnd, a.data(), b.data(), MB_OK | MB_ICONWARNING);

			return;
		}

		isEditNote = true;
		prepareNoteDialog();
		ShowWindow(dialogAddNote, SW_SHOW);
		break;

	case IDC_BTN_STATISTICS:
		if (isStatistics)
		{
			ShowWindow(treeTag, SW_SHOW);
			ShowWindow(listNote, SW_SHOW);
			ShowWindow(txtNoteCount, SW_SHOW);
			ShowWindow(btnTagRename, SW_SHOW);
			ShowWindow(btnTagDelete, SW_SHOW);
			ShowWindow(btnNoteAdd, SW_SHOW);
			ShowWindow(btnNoteEdit, SW_SHOW);
			ShowWindow(btnNoteDelete, SW_SHOW);
			ShowWindow(groupTag, SW_SHOW);
			ShowWindow(groupNote, SW_SHOW);

			isStatistics = false;

			SetWindowText(btnStatistics, STATISTICS.c_str());

			int size = chartHWND.size();
			for (int i = 0; i < size; i++)
			{
				ShowWindow(chartHWND[i], SW_HIDE);
			}
		}
		else
		{
			ShowWindow(treeTag, SW_HIDE);
			ShowWindow(listNote, SW_HIDE);
			ShowWindow(txtNoteCount, SW_HIDE);
			ShowWindow(btnTagRename, SW_HIDE);
			ShowWindow(btnTagDelete, SW_HIDE);
			ShowWindow(btnNoteAdd, SW_HIDE);
			ShowWindow(btnNoteEdit, SW_HIDE);
			ShowWindow(btnNoteDelete, SW_HIDE);
			ShowWindow(groupTag, SW_HIDE);
			ShowWindow(groupNote, SW_HIDE);

			isStatistics = true;

			SetWindowText(btnStatistics, VIEW_NOTE.c_str());
		}

		// update paint
		HWND hwnd = GetForegroundWindow();
		InvalidateRect(hwnd, NULL, TRUE);
		UpdateWindow(hwnd);
		break;
	}
}

void OnPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc;
	hdc = BeginPaint(hWnd, &ps);

	// get system font
	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	HFONT hFont = CreateFont(lf.lfHeight, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, VIETNAMESE_CHARSET,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);


	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	HFONT hFont_big = CreateFont(25, lf.lfWidth,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, VIETNAMESE_CHARSET,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);

	if (isStatistics)
	{
		vector<wstring>list_data;
		vector<int>list_value;
		int list_size, max_value, total_value;

		HWND hwnd;

		HRGN hRegion;
		HBRUSH hbrush;
		HPEN hPen;

		float height, width;
		int x, y;

		list_size = 5;

		height = 300;	// chart size
		width = 350;

		x = 50;			// start position
		y = 350;

		// 
		hPen = CreatePen(PS_COSMETIC, 5, RGB(121, 85, 72));
		SelectObject(hdc, hPen);

		// horizontal axis
		MoveToEx(hdc, x, y, NULL);
		LineTo(hdc, x + width, y);

		MoveToEx(hdc, x + width, y, NULL);
		LineTo(hdc, x + width - 10, y + 10);

		MoveToEx(hdc, x + width, y, NULL);
		LineTo(hdc, x + width - 10, y - 10);

		// vertical axis
		height = height + 30; // higher for axis
		MoveToEx(hdc, x, y, NULL);
		LineTo(hdc, x, y - height);

		MoveToEx(hdc, x, y - height, NULL);
		LineTo(hdc, x + 10, y - height + 10);

		MoveToEx(hdc, x, y - height, NULL);
		LineTo(hdc, x - 10, y - height + 10);

		height = height - 30;

		// draw dot on axis
		for (int i = 0; i < 11; i++)
		{
			MoveToEx(hdc, x, y - 30 * i, NULL);
			LineTo(hdc, x - 2, y - 30 * i);
		}

		DeleteObject(hPen);

		// ---------------------------- get data to draw ----------------------------
		list_size = myTags.getDataForChart(list_data, list_value);
		if (list_size > 5)
		{
			list_size = 5;
		}

		max_value = 0;
		for (int i = 0; i < list_size; i++)
		{
			if (list_value[i] > max_value)
			{
				max_value = list_value[i];
			}
		}

		// draw chart
		for (int i = 0; i < list_size; i++)
		{
			if (i == 0)
			{
				x = x + 30;
			}
			else
			{
				x = x + 60;
			}

			hRegion = CreateRectRgn(x, y - 2, x + 30, y - ((float)list_value[i] / (float)max_value * height));
			hbrush = CreateSolidBrush(CHART_COLOR[i]);
			FillRgn(hdc, hRegion, hbrush);

			hwnd = CreateWindowEx(0, L"STATIC", (to_wstring(list_value[i])).c_str(), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, x, y - ((float)list_value[i] / (float)max_value * height) - 20, 30, 20, hWnd, NULL, hInst, NULL);
			SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);
			chartHWND.push_back(hwnd);
		}

		// draw caption
		x = x + 65;
		y = y + 10;

		hwnd = CreateWindowEx(0, L"STATIC", TAG_STATISTICS.c_str(), WS_CHILD | WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, x + 15 + CAPTION_EDGE, y - height - CAPTION_EDGE, 200, 50, hWnd, NULL, hInst, NULL);
		SendMessage(hwnd, WM_SETFONT, WPARAM(hFont_big), TRUE);
		chartHWND.push_back(hwnd);

		x = x + 35;
		y = y + 30;
		for (int i = 0; i < list_size; i++)
		{
			y = y + CAPTION_SPACE;
			hRegion = CreateRectRgn(x, y - height, x + CAPTION_EDGE, y - height - CAPTION_EDGE);
			hbrush = CreateSolidBrush(CHART_COLOR[i]);
			FillRgn(hdc, hRegion, hbrush);

			hwnd = CreateWindowEx(0, L"STATIC", list_data[i].c_str(), WS_CHILD | WS_VISIBLE | SS_LEFT | SS_CENTERIMAGE, x + 15 + CAPTION_EDGE, y - height - CAPTION_EDGE, 200, CAPTION_EDGE, hWnd, NULL, hInst, NULL);
			SendMessage(hwnd, WM_SETFONT, WPARAM(hFont), TRUE);
			chartHWND.push_back(hwnd);
		}


		// total
		total_value = 0;
		for (int i = 0; i < list_size; i++)
		{
			total_value += list_value[i];
		}

		y = y + CAPTION_SPACE;
		hwnd = CreateWindowEx(0, L"STATIC", (TOTAL + to_wstring(total_value)).data(), WS_CHILD | WS_VISIBLE | SS_LEFT | SS_CENTERIMAGE, x + 15 + CAPTION_EDGE, y - height - CAPTION_EDGE, 200, CAPTION_EDGE, hWnd, NULL, hInst, NULL);
		chartHWND.push_back(hwnd);
	}

	EndPaint(hWnd, &ps);
}

void OnDestroy(HWND hwnd)
{
	// release noti
	Shell_NotifyIcon(NIM_DELETE, &nidApp);

	// save all data if crash
	myNotes.writeData();
	myTags.writeData();

	// hook
	doRemoveHook(dialogAddNote);


	PostQuitMessage(0);
}