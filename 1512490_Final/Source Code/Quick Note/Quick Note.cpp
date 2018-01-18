// Quick Note.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Quick Note.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

WCHAR szlTitle[MAX_LOADSTRING];                  // The title bar listview text
WCHAR szlWindowClass[MAX_LOADSTRING];            // listview window class name

WCHAR szsTitle[MAX_LOADSTRING];             // statitics window
WCHAR szsWindowClass[MAX_LOADSTRING];            //statitic window class name

WCHAR szanTitle[MAX_LOADSTRING];             // add note window
WCHAR szanWindowClass[MAX_LOADSTRING];            //add note window class name

std::vector<CNote> PreviewNotes;
std::vector<std::wstring> AllPreviewNote;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

//Self-defined forward declarations of functions included in this code module:
BOOL				OnInitDialog(HWND hWnd);
void				ShowContextMenu(HWND hWnd);
int CreateAddNotesWindow(HINSTANCE hInstance, int nCmdShow, HWND hWnd, LPWSTR WindowClass, int x, int y);
int CreateListWindow(HINSTANCE hInstance, int nCmdShow, HWND hWnd, LPWSTR WindowClass, int x, int y);
int CreateStatiticsWindow(HINSTANCE hInstance, int nCmdShow);
int CreateSeparateListWindow(HINSTANCE hInstance, int nCmdShow);
int CreateSeparateAddNoteWindow(HINSTANCE hInstance, int nCmdShow);
int ChangeFont(HWND hwnd);

BOOL InitInstanceForListView(HINSTANCE hInstance, int nCmdShow);
ATOM ListRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK WndListProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
int loadTagToListView(HWND hwndListView);

int InitPie(CPiePart * PiePart, int & index, float percentage);
std::wstring AttatchResultString(std::wstring notificationString, float input);
int drawPieChart(HINSTANCE hInstance, int nCmdShow, HWND hWnd, HDC & hdc, int x, int y);
int CreatePiePart(CPiePart * PiePart, HINSTANCE hInstance, HWND hWnd, int x, int y);

BOOL InitInstanceForStatitics(HINSTANCE hInstance, int nCmdShow);
ATOM StatiticsRegisterClass(HINSTANCE hInstance);
LRESULT CALLBACK WndStatiticsProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

CNote getNoteFromWindow(HWND hWnd);
std::vector<std::wstring> getWindowNoteTag(HWND hWnd);
LPWSTR getWindowNoteContent(HWND hWnd);
LPWSTR getWindowNoteTitle(HWND hWnd);
std::vector<std::wstring> separateTagFromText(LPWSTR TagsLine);
std::vector<std::wstring> getAllPreviewNotesContent(std::wstring TagName);
std::wstring getCurrentTag(HWND hWnd, int uID);
int showPreviewNote(std::vector<std::wstring> nContents, HWND hWnd, WPARAM ID);
std::vector<CNote> getPreviewNotes(std::wstring Tag);
int getCurrentID(HWND hWnd, int wid, std::vector<CNote> PreviewNotes);
int isTheSameString(LPWSTR fiStr, LPWSTR secString);

int showNote(HWND hWnd, int nid, int ticounter, int ccounter, int tacounter);
int AddTitle(HWND hWnd, std::wstring Title, int counter);
int AddContent(HWND hWnd, std::wstring Content, int counter);
int AddTags(HWND hWnd, std::vector<std::wstring> Tags, int counter);

BOOL InitInstanceForAddNote(HINSTANCE hInstance, int nCmdShow);

ATOM AddNoteRegisterClass(HINSTANCE hInstance);

LRESULT CALLBACK WndAddProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);



//Self-defined global variables
NOTIFYICONDATA	niData;	// notify icon data
CNotesHolder nHolder;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
	NOTIFYICONDATA niData; //Contains data that the system needs to display notifications in the notification area.

	if (CNotificationHelper::isFirstTimeStartUp()) {
		CNotificationHelper::setupApplication();
	}
	else {
		// Havent figured out what to do...
		//Okay I figured it out :))
		nHolder.importNotes();
	}


	BOOL okay = RegisterHotKey(NULL, 609, MOD_ALT | MOD_NOREPEAT, VK_SPACE);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_QUICKNOTE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
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

		if (msg.message == WM_HOTKEY)
		{
			CreateSeparateAddNoteWindow(hInst, SW_RESTORE);
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_QUICKNOTE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_QUICKNOTE);
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
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ChangeFont(hWnd);

	// Fill the NOTIFYICONDATA structure and call Shell_NotifyIcon

	ZeroMemory(&niData, sizeof(NOTIFYICONDATA)); //niData = NULL

	niData.cbSize = sizeof(NOTIFYICONDATA);

	// set ID number for the tray icon
	niData.uID = TRAYICONID;

	// state which structure members are valid
	niData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;

	/*NIF_ICON (0x00000002)
	0x00000002. The hIcon member is valid.
	NIF_TIP (0x00000004)
	0x00000004. The szTip member is valid.
	NIF_MESSAGE (0x00000001)
	0x00000001. The uCallbackMessage member is valid.
	*/

	// load the icon
	niData.hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_QUICKNOTE),
		IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON),
		LR_DEFAULTCOLOR);

	// the window to send messages to and the message to send
	//		note:	the message value should be in the
	//				range of WM_APP through 0xBFFF
	niData.hWnd = hWnd;
	niData.uCallbackMessage = SWM_TRAYMSG;
	// tooltip message
	lstrcpyn(niData.szTip, _T("Link githup: github.com/nhthanhtan/windev.git"), sizeof(niData.szTip) / sizeof(TCHAR));

	Shell_NotifyIcon(NIM_ADD, &niData);

	// free icon handle
	if (niData.hIcon && DestroyIcon(niData.hIcon))
		niData.hIcon = NULL;

	//Create window
	CreateAddNotesWindow(hInstance, nCmdShow, hWnd, szWindowClass, 10, 10);
	CreateListWindow(hInstance, nCmdShow, hWnd, szWindowClass, 400, 10);


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

	case WM_HOTKEY: {
		case QUICK_KEY: {
			CreateSeparateAddNoteWindow(hInst, SW_RESTORE);
			break;
		}
	}
	case SWM_TRAYMSG:
		switch (lParam)
		{
		case WM_LBUTTONDBLCLK:
			ShowWindow(hWnd, SW_RESTORE);
			break;
		case WM_RBUTTONDOWN:
		case WM_CONTEXTMENU:
			ShowContextMenu(hWnd);
		}
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {

			case SWM_SHOW:
				ShowWindow(hWnd, SW_RESTORE);
				break;
			case SWM_HIDE:
				ShowWindow(hWnd, SW_HIDE);
				break;
			case SWM_VIEW_NOTES:
				CreateSeparateListWindow(hInst, SW_RESTORE);
				break;
			case SWM_VIEW_STATS:
				CreateStatiticsWindow(hInst, SW_RESTORE);
				break;
			case IDOK:
				ShowWindow(hWnd, SW_HIDE);
				break;
			case SWM_EXIT:
				DestroyWindow(hWnd);
				break;
			case IDB_SAVE_NOTE: {
				//Save Note
				CNote tmp;
				tmp = getNoteFromWindow(hWnd);
				nHolder.addNote(tmp);
				nHolder.exportNotes();
				nHolder.importNotes();
				InvalidateRect(hWnd, NULL, TRUE);
				UpdateWindow(hWnd);
				break;
			}
			case IDB_SHOW_PREVIEW_NOTE_UP: {
				std::wstring curTag = getCurrentTag(hWnd, ID_TAG_LIST_UP);
				PreviewNotes.empty();
				PreviewNotes = getPreviewNotes(curTag);
				std::vector<std::wstring> AllPreviewNote = getAllPreviewNotesContent(curTag);
				/*Get preview handler*/
				HWND hwndtmp = NULL;
				for (int i = 0; i < 5; i++) {
					hwndtmp = FindWindowEx(hWnd, hwndtmp, NULL, NULL);
				}
				showPreviewNote(AllPreviewNote, hwndtmp, (WPARAM) ID_NOTE_PREVIEW_LIST);
				break;
			}
			case IDB_SHOW_PREVIEW_NOTE_DOWN: {
				std::wstring curTag = getCurrentTag(hWnd, ID_TAG_LIST_DOWN);
				PreviewNotes.empty();
				PreviewNotes = getPreviewNotes(curTag);
				std::vector<std::wstring> AllPreviewNote = getAllPreviewNotesContent(curTag);
				/*Get preview handler*/
				HWND hwndtmp = NULL;
				for (int i = 0; i < 5; i++) {
					hwndtmp = FindWindowEx(hWnd, hwndtmp, NULL, NULL);
				}
				showPreviewNote(AllPreviewNote, hwndtmp, (WPARAM)ID_NOTE_PREVIEW_LIST);
				break;
			}
			case IDB_SHOW_NOTE: {
				int ID;
				ID = getCurrentID(hWnd, ID_NOTE_PREVIEW_LIST, PreviewNotes);
				showNote(hWnd, ID, 6 , 8, 9);
				break;
			}
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
	case WM_INITDIALOG:
		return OnInitDialog(hWnd);
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
			drawPieChart(nullptr, NULL, hWnd, hdc, 800, 100);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
		nHolder.exportNotes();
		niData.uFlags = 0;
		Shell_NotifyIcon(NIM_DELETE, &niData);
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

BOOL OnInitDialog(HWND hWnd)
{
	HMENU hMenu = GetSystemMenu(hWnd, FALSE);
	if (hMenu)
	{
		AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
		AppendMenu(hMenu, MF_STRING, IDM_ABOUT, _T("About"));
	}
	HICON hIcon = (HICON)LoadImage(hInst,
		MAKEINTRESOURCE(IDI_QUICKNOTE),
		IMAGE_ICON, 0, 0, LR_SHARED | LR_DEFAULTSIZE);
	SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
	return TRUE;
}

void ShowContextMenu(HWND hWnd)
{
	POINT pt;
	GetCursorPos(&pt);
	HMENU hMenu = CreatePopupMenu();
	if (hMenu)
	{
		if (IsWindowVisible(hWnd))
			InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_HIDE, _T("Hide"));
		else
			InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_SHOW, _T("Show"));
		InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_VIEW_NOTES, _T("View Notes"));
		InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_VIEW_STATS, _T("View Statitics"));
		InsertMenu(hMenu, -1, MF_BYPOSITION, SWM_EXIT, _T("Exit"));

		// note:	must set window to the foreground or the
		//			menu won't disappear when it should
		SetForegroundWindow(hWnd);

		TrackPopupMenu(hMenu, TPM_BOTTOMALIGN,
			pt.x, pt.y, 0, hWnd, NULL);
		DestroyMenu(hMenu);
	}
}

int CreateAddNotesWindow(HINSTANCE hInstance, int nCmdShow, HWND hWnd, LPWSTR WindowClass, int x, int y)
{
	try {
		HWND hwndAddItemWindow = CreateWindow(WindowClass, L"Add Notes controll window",
			WS_THICKFRAME | WS_MINIMIZEBOX |
			WS_MAXIMIZEBOX | WS_VISIBLE | WS_CHILD,
			x, y, 350, 450, hWnd,
			(HMENU)IDD_ADD_NOTES_CONTROL_WINDOW, hInstance, NULL);

		HWND hwndAddNotesWindow = CreateWindow(L"Edit", L"Adding Notes",
			WS_THICKFRAME | WS_MINIMIZEBOX |
			WS_MAXIMIZEBOX | WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL | ES_MULTILINE,
			x, y + 100, 150, 300, hwndAddItemWindow,
			(HMENU)IDD_ADD_NOTES, hInstance, NULL);

		HWND hwndAddTagWindow = CreateWindow(L"Edit", L"Adding Tags",
			WS_THICKFRAME | WS_MINIMIZEBOX |
			WS_MAXIMIZEBOX | WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL | ES_MULTILINE,
			x + 150, y, 150, 300, hwndAddItemWindow,
			(HMENU)IDD_ADD_TAGS, hInstance, NULL);

		HWND hwndAddTitleWindow = CreateWindow(L"Edit", L"Adding Title",
			WS_THICKFRAME | WS_MINIMIZEBOX |
			WS_MAXIMIZEBOX | WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL | ES_MULTILINE,
			x, y, 150, 100, hwndAddItemWindow,
			(HMENU)IDD_ADD_TITLES, hInstance, NULL);

		HWND hwndSaveButton = CreateWindow(L"BUTTON", L"Save",
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			x + 200, y + 330, 50, 50, hwndAddItemWindow,
			(HMENU)IDB_SAVE_NOTE, hInstance, NULL);

		ChangeFont(hwndAddItemWindow);
		ChangeFont(hwndAddTagWindow);
		ChangeFont(hwndAddNotesWindow);
		ChangeFont(hwndAddTitleWindow);
		ChangeFont(hwndSaveButton);

		return 1;
	}
	catch (const std::exception&) {
		return 0;
	}
}

int CreateListWindow(HINSTANCE hInstance, int nCmdShow, HWND hWnd, LPWSTR WindowClass, int x, int y)
{
	try {
		/*Preview Tag group*/
		HWND hwndStaticTagName = CreateWindow(L"STATIC", L"Tags List",
			WS_CHILDWINDOW | WS_VISIBLE | WS_ACTIVECAPTION,
			x, y, 60, 20, hWnd,
			nullptr, hInstance, NULL);

		HWND hwndListView = CreateWindow(L"COMBOBOX", L"Tags List",
			CBS_DROPDOWNLIST | WS_CHILDWINDOW | WS_VISIBLE | ES_AUTOVSCROLL | ES_MULTILINE,
			x + 70, y, 150, 300, hWnd,
			(HMENU)ID_TAG_LIST_UP, hInstance, NULL);

		HWND hwndShowPreviewButton = CreateWindow(L"BUTTON", L"Show Preview",
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			x + 220, y, 90, 20, hWnd,
			(HMENU)IDB_SHOW_PREVIEW_NOTE_UP, hInstance, NULL);
		/*End preview Tag group*/

		HWND hwndNotePreviewList = CreateWindow(L"COMBOBOX", L"Tags Preview List",
			CBS_DROPDOWNLIST | WS_CHILDWINDOW | WS_VISIBLE | ES_AUTOVSCROLL | ES_MULTILINE,
			x, y + 50, 310, 300, hWnd,
			(HMENU)ID_NOTE_PREVIEW_LIST, hInstance, NULL);

		HWND hwndNoteTitle = CreateWindow(L"Edit", L"Title",
			WS_CHILDWINDOW | WS_VISIBLE | WS_ACTIVECAPTION | WS_THICKFRAME | WS_MINIMIZEBOX |
			WS_MAXIMIZEBOX | WS_CHILD | ES_AUTOVSCROLL | ES_MULTILINE,
			x, y + 150, 310, 40, hWnd,
			(HMENU)ID_NOTE_TITLE, hInstance, NULL);

		HWND hwndShowNote = CreateWindow(L"BUTTON", L"Show Full Note",
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			x + 100, y + 90, 90, 40, hWnd,
			(HMENU)IDB_SHOW_NOTE, hInstance, NULL);
		
		HWND hwndNoteContent = CreateWindow(L"Edit", L"Content",
			WS_CHILDWINDOW | WS_VISIBLE | WS_ACTIVECAPTION | WS_THICKFRAME | WS_MINIMIZEBOX |
			WS_MAXIMIZEBOX | WS_CHILD  | ES_AUTOVSCROLL | ES_MULTILINE,
			x, y + 200, 310, 200, hWnd,
			(HMENU)ID_NOTE_CONTENT, hInstance, NULL);
		/*Preview Tag group*/

		HWND hwndNoteTags = CreateWindow(L"COMBOBOX", L"Tags List",
			CBS_DROPDOWNLIST | WS_CHILDWINDOW | WS_VISIBLE | ES_AUTOVSCROLL | ES_MULTILINE,
			x + 70, y + 405, 150, 300, hWnd,
			(HMENU)ID_TAG_LIST_DOWN, hInstance, NULL);

		HWND hwndStaticCurrentTagName = CreateWindow(L"STATIC", L"Note's Tags",
			WS_CHILDWINDOW | WS_VISIBLE | WS_ACTIVECAPTION,
			x, y + 405, 60, 20, hWnd,
			nullptr, hInstance, NULL);

		HWND hwndShowPreviewButtonDown = CreateWindow(L"BUTTON", L"Show Preview",
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			x + 220, y + 405, 90, 20, hWnd,
			(HMENU)IDB_SHOW_PREVIEW_NOTE_DOWN, hInstance, NULL);

		/*End preview Tag group*/
		
		ChangeFont(hwndListView);
		ChangeFont(hwndShowPreviewButton);
		ChangeFont(hwndStaticTagName);
		ChangeFont(hwndNoteTitle);
		ChangeFont(hwndNoteContent);
		ChangeFont(hwndNoteTags);
		ChangeFont(hwndStaticCurrentTagName);
		ChangeFont(hwndShowPreviewButtonDown);
		ChangeFont(hwndNotePreviewList);
		ChangeFont(hwndShowNote);

		loadTagToListView(hwndListView);
		return 1;
	}
	catch (const std::exception&) {
		return 0;
	}
}

int CreateStatiticsWindow(HINSTANCE hInstance, int nCmdShow)
{
	// Initialize global strings
	LoadStringW(hInstance, IDS_STATITICS_TITLE, szsTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_STATITICS, szsWindowClass, MAX_LOADSTRING);
	StatiticsRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstanceForStatitics(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_STATITICS));

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


	return 1;
}

int CreateSeparateListWindow(HINSTANCE hInstance, int nCmdShow)
{
	// Initialize global strings
	LoadStringW(hInstance, IDS_TAG_LIST_SEPARATE_TITLE, szlTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_TAG_LIST_SEPARATE, szlWindowClass, MAX_LOADSTRING);
	ListRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstanceForListView(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TAG_LIST_SEPARATE));

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

	
	return 1;
}

int CreateSeparateAddNoteWindow(HINSTANCE hInstance, int nCmdShow) {

	// Initialize global strings
	LoadStringW(hInstance, IDS_ADD_NOTE_SEPARATE_TITLE, szanTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_ADD_NOTE_SEPARATE, szanWindowClass, MAX_LOADSTRING);
	AddNoteRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstanceForAddNote(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ADD_NOTE_SEPARATE));

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


	return 1;
}

int AddItemToBox(HWND hwnd, LPARAM text)
{
	try
	{
		SendMessage(hwnd, (UINT)CB_ADDSTRING, (WPARAM)0, text);
		return 1;
	}
	catch (const std::exception&)
	{
		throw;
		return 0;
	}
}

int ChangeFont(HWND hwnd) {

	try {
		HFONT hFont = CreateFont(13, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET,
			OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH | FF_DONTCARE, DEFAULT_FONT);

		SendMessage(hwnd, WM_SETFONT, (WPARAM)hFont, TRUE);

		return 1;
	}
	catch (const std::exception&) {
		throw;
		return 0;
	}


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
BOOL InitInstanceForListView(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szlWindowClass, szlTitle, WS_OVERLAPPEDWINDOW,
		0, 0, 350, 530, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	CreateListWindow(hInstance, nCmdShow, hWnd, szlWindowClass, 10, 10);

	ChangeFont(hWnd);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM ListRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndListProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_QUICKNOTE));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_QUICKNOTE);
	wcex.lpszClassName = szlWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

LRESULT CALLBACK WndListProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDOK:
			ShowWindow(hWnd, SW_HIDE);
			break;
		case SWM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDB_SHOW_PREVIEW_NOTE_UP: {
			std::wstring curTag = getCurrentTag(hWnd, ID_TAG_LIST_UP);
			PreviewNotes.empty();
			PreviewNotes = getPreviewNotes(curTag);
			std::vector<std::wstring> AllPreviewNote = getAllPreviewNotesContent(curTag);
			/*Get preview handler*/
			HWND hwndtmp = NULL;
			for (int i = 0; i < 4; i++) {
				hwndtmp = FindWindowEx(hWnd, hwndtmp, NULL, NULL);
			}

			showPreviewNote(AllPreviewNote, hwndtmp, (WPARAM)ID_NOTE_PREVIEW_LIST);
			break;
		}
		case IDB_SHOW_PREVIEW_NOTE_DOWN: {
			std::wstring curTag = getCurrentTag(hWnd, ID_TAG_LIST_DOWN);
			PreviewNotes.empty();
			PreviewNotes = getPreviewNotes(curTag);
			std::vector<std::wstring> AllPreviewNote = getAllPreviewNotesContent(curTag);
			/*Get preview handler*/
			HWND hwndtmp = NULL;
			for (int i = 0; i < 4; i++) {
				hwndtmp = FindWindowEx(hWnd, hwndtmp, NULL, NULL);
			}
			showPreviewNote(AllPreviewNote, hwndtmp, (WPARAM)ID_NOTE_PREVIEW_LIST);
			break;
		}
		case IDB_SHOW_NOTE: {
			int ID;
			ID = getCurrentID(hWnd, ID_NOTE_PREVIEW_LIST, PreviewNotes);
			showNote(hWnd, ID, 5, 7, 8);
			break;
		}
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

int loadTagToListView(HWND hwndListView) {
	for (int i = 0; i < nHolder.getDistincTag().size(); i++) {
		std::vector<std::wstring> DistincTags = nHolder.getDistincTag();
		AddItemToBox(hwndListView, (LPARAM)const_cast <LPWSTR> (DistincTags[i].c_str()));
	}

	SendMessage(hwndListView, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

	return 1;
}

int InitPie(CPiePart *PiePart, int &index, float percentage)
{
	if (0 == index) {

		float swAng = roundf(360.0 * percentage);
		PiePart[0].setStartAngle(0);
		PiePart[0].setSweepAngle(swAng);

	}
	else {

		float preStAng = PiePart[index - 1].getStartAngle();
		float preSwAng = PiePart[index - 1].getSweepAngle();
		float stAng = preStAng + preSwAng;
		float swAng = roundf(360.0 * percentage);
		PiePart[index].setStartAngle(stAng);
		PiePart[index].setSweepAngle(swAng);

	}


	index++;
	return 0;
}

std::wstring AttatchResultString(std::wstring notificationString, float input) {

	std::wostringstream oss;
	oss << std::fixed << std::setprecision(2) << input;

	std::wstring ws = oss.str();

	std::wstring result;

	result = notificationString + ws;

	return result;
}

int drawPieChart(HINSTANCE hInstance, int nCmdShow, HWND hWnd, HDC &hdc, int x, int y)
{
	int slideCount = nHolder.getDistincTag().size();
	CPiePart *PiePart = new CPiePart[slideCount];

	for (int i = 0; i < slideCount; i++) {
	}

	CreatePiePart(PiePart, hInstance, hWnd, x, y);

	for (int i = 0; i < slideCount; i++) {
		
		PiePart[i].setX(x);
		PiePart[i].setY(y);

		hdc = GetDC(hWnd);
		BeginPath(hdc);

		SelectObject(hdc, CreateSolidBrush(RGB(0 + i*34, 0 + i*19, 0 + i*37)));

		MoveToEx(hdc, PiePart[i].getX(), PiePart[i].getY(), (LPPOINT)NULL);
		AngleArc(hdc, PiePart[i].getX(), PiePart[i].getY(), PiePart[i].getR(),
			PiePart[i].getStartAngle(), PiePart[i].getSweepAngle());
		LineTo(hdc, PiePart[i].getX(), PiePart[i].getY());
		EndPath(hdc);
		StrokeAndFillPath(hdc);
		ReleaseDC(hWnd, hdc);
	}

	return 0;
}

int CreatePiePart(CPiePart * PiePart, HINSTANCE hInstance, HWND hWnd, int x, int y)
{
	int slideCount = nHolder.getDistincTag().size();
	int index = 0;

	float total = 0;

	std::vector<int> TagCountHolder = nHolder.getDistincTagCount();
	std::vector<std::wstring> TagsName = nHolder.getDistincTag();

	for (int i = 0; i < slideCount; i++) {
		total = total + TagCountHolder[i];
	}

	if (0 == total) {
		return 0;
	}

	HWND hwndTagCountPercentage = CreateWindow(L"COMBOBOX", L"Tag Count",
		WS_CHILDWINDOW | WS_VISIBLE | ES_AUTOVSCROLL | ES_MULTILINE,
		x, y + 100, 200, 100, hWnd,
		NULL, hInstance, NULL);

	for (int i = 0; i < slideCount; i++) {
		float percentage = (float)TagCountHolder[i] / total;
		std::wstring wtmp = TagsName[i] + L" ";
		wtmp = AttatchResultString(const_cast <LPWSTR> (wtmp.c_str()), percentage * 100);
		wtmp = wtmp + L"%" + L" Corresponding notes: " + std::to_wstring(TagCountHolder[i]);
		AddItemToBox(hwndTagCountPercentage, (LPARAM) const_cast <LPWSTR> (wtmp.c_str()));
		InitPie(PiePart, index, percentage);
	}

	SendMessage(hwndTagCountPercentage, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);

	ChangeFont(hwndTagCountPercentage);

	return 1;
}

BOOL InitInstanceForStatitics(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szsWindowClass, szsTitle, WS_OVERLAPPEDWINDOW,
		0, 0, 350, 530, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ChangeFont(hWnd);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

ATOM StatiticsRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndStatiticsProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_QUICKNOTE));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_QUICKNOTE);
	wcex.lpszClassName = szsWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

LRESULT CALLBACK WndStatiticsProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDOK:
			ShowWindow(hWnd, SW_HIDE);
			break;
		case SWM_EXIT:
			DestroyWindow(hWnd);
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
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		drawPieChart(nullptr, NULL, hWnd, hdc, 100, 100);
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

CNote getNoteFromWindow(HWND hWnd) {
	CNote *note;
	note = new CNote;

	note->setTags(getWindowNoteTag(hWnd));
	note->setContent(getWindowNoteContent(hWnd));
	note->setTitle(getWindowNoteTitle(hWnd));
	note->setID(nHolder.getNoteCount());
	return *note;
}

std::vector<std::wstring> getWindowNoteTag(HWND hWnd) {
	LPWSTR tmp = new WCHAR[MAX_LINE_BUFFER];
	GetDlgItemTextW(hWnd, IDD_ADD_TAGS, tmp, MAX_LINE_BUFFER);
	std::vector<std::wstring> result = separateTagFromText(tmp);
	return result;

}

LPWSTR getWindowNoteContent(HWND hWnd) {
	LPWSTR tmp = new WCHAR[MAX_CONTENT_BUFFER];
	GetDlgItemTextW(hWnd, IDD_ADD_NOTES, tmp, MAX_CONTENT_BUFFER);
	return tmp;
}

LPWSTR getWindowNoteTitle(HWND hWnd) {
	LPWSTR tmp = new WCHAR[MAX_LINE_BUFFER];
	GetDlgItemTextW(hWnd, IDD_ADD_TITLES, tmp, MAX_LINE_BUFFER);
	return tmp;
}

std::vector<std::wstring> separateTagFromText(LPWSTR TagsLine) {
	//std::vector <LPWSTR> tmp;

	std::vector<std::wstring> split_tags;
	std::wstringstream ss;
	ss.str(TagsLine);
	std::wstring tag;
	while (std::getline(ss, tag, L',')) {
		//split_tags.push_back(std::wstring(tag.begin(), tag.end()));
		split_tags.push_back(std::wstring(tag));
	}

	/*
	for (int i = 0; i < split_tags.size(); i++) {
		LPWSTR temp;
		temp = (LPWSTR)split_tags[i].c_str();
		tmp.push_back(temp);
	}*/

	return split_tags;
}

std::vector<std::wstring> getAllPreviewNotesContent(std::wstring TagName) {
	std::vector<std::wstring> nContent;

	for (int i = 0; i < nHolder.getNoteCount(); i++) {
		if (nHolder.getNote(i).isExistTag(TagName)) {
			std::wstring tmp;
			CNote ntmp;
			ntmp = nHolder.getNote(i);
			tmp = ntmp.getContent();
			nContent.push_back(tmp);
		}
		else {
			//do nothing
		}
	}

	return nContent;
}

std::wstring getCurrentTag(HWND hWnd, int uID) {
	LPWSTR lptmp;
	lptmp = new WCHAR[MAX_LINE_BUFFER];
	GetDlgItemTextW(hWnd, uID, lptmp, MAX_LINE_BUFFER);
	std::wstring wtmp = lptmp;
	return wtmp;
}

int showPreviewNote(std::vector<std::wstring> nContents, HWND hWnd, WPARAM ID) {

	SendMessage(hWnd, CB_RESETCONTENT, (WPARAM)0, (LPARAM)0);
	for (int i = 0; i < nContents.size(); i++) {
		LPWSTR ktmp = const_cast<LPWSTR> (nContents[i].c_str());
		SendMessageW(hWnd, CB_ADDSTRING, ID, (LPARAM) ktmp);
	}
	SendMessage(hWnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
	return 1;
}

std::vector<CNote> getPreviewNotes(std::wstring Tag) {

	std::vector<CNote> tmpNote;

	for (int i = 0; i < nHolder.getNoteCount(); i++) {
		if (nHolder.getNote(i).isExistTag(Tag)) {
			std::wstring tmp;
			CNote ntmp;
			ntmp = nHolder.getNote(i);
			tmpNote.push_back(ntmp);
		}
		else {
			//do nothing
		}
	}

	return tmpNote;

}

int getCurrentID(HWND hWnd, int wid, std::vector<CNote> PreviewNotes) {


	LPWSTR lptmp = new WCHAR [MAX_CONTENT_BUFFER];
	GetDlgItemTextW(hWnd, wid, lptmp, MAX_CONTENT_BUFFER);

	for (int i = 0; i < PreviewNotes.size(); i++) {
		std::wstring tmp = PreviewNotes[i].getContent();
		//LPWSTR lptmp2 = const_cast<LPWSTR>(PreviewNotes[i].getContent().c_str());
		LPWSTR lptmp2 = const_cast<LPWSTR>(tmp.c_str());
		if (isTheSameString(lptmp, lptmp2)) {
			return PreviewNotes[i].getID();
		}
	}
}

int isTheSameString(LPWSTR fiStr, LPWSTR secString) {
	std::wstring tmp1 = fiStr;
	std::wstring tmp2 = secString;

	if (tmp1.compare(tmp2) == 0)
		return 1;

	return 0;
}

int showNote(HWND hWnd, int nid, int ticounter, int ccounter, int tacounter) {
	CNote tmp;
	tmp = nHolder.getNote(nid);
	AddTitle(hWnd, tmp.getTitle(), ticounter);
	AddContent(hWnd, tmp.getContent(), ccounter);
	AddTags(hWnd, tmp.getTags(), tacounter);
	return 1;
}

int AddTitle(HWND hWnd, std::wstring Title, int counter){
	HWND hwndtmp = NULL;
	for (int i = 0; i < counter; i++) {
		hwndtmp = FindWindowEx(hWnd, hwndtmp, NULL, NULL);
	}
	SendMessage(hwndtmp, WM_SETTEXT, (WPARAM)0, (LPARAM) const_cast <LPWSTR> (Title.c_str()));
	return 1;
}

int AddContent(HWND hWnd, std::wstring Content, int counter) {
	HWND hwndtmp = NULL;
	for (int i = 0; i < counter; i++) {
		hwndtmp = FindWindowEx(hWnd, hwndtmp, NULL, NULL);
	}
	SendMessage(hwndtmp, WM_SETTEXT, (WPARAM)0, (LPARAM) const_cast <LPWSTR> (Content.c_str()));
	return 1;
}

int AddTags(HWND hWnd, std::vector<std::wstring> Tags, int counter) {
	HWND hwndtmp = NULL;
	for (int i = 0; i < counter; i++) {
		hwndtmp = FindWindowEx(hWnd, hwndtmp, NULL, NULL);
	}
	SendMessage(hwndtmp, CB_RESETCONTENT, (WPARAM)0, (LPARAM)0);
	for (int i = 0; i < Tags.size(); i++) {
		LPWSTR ktmp = const_cast<LPWSTR> (Tags[i].c_str());
		SendMessageW(hwndtmp, CB_ADDSTRING, (WPARAM) 0, (LPARAM)ktmp);
	}
	SendMessage(hwndtmp, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
	return 1;
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
BOOL InitInstanceForAddNote(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szanWindowClass, szanTitle, WS_OVERLAPPEDWINDOW,
		0, 0, 350, 530, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	CreateAddNotesWindow(hInstance, nCmdShow, hWnd, szanWindowClass, 10, 10);

	ChangeFont(hWnd);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM AddNoteRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndAddProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_QUICKNOTE));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_QUICKNOTE);
	wcex.lpszClassName = szanWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

LRESULT CALLBACK WndAddProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDOK:
			ShowWindow(hWnd, SW_HIDE);
			break;
		case SWM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDB_SAVE_NOTE: {
			//Save Note
			CNote tmp;
			tmp = getNoteFromWindow(hWnd);
			nHolder.addNote(tmp);
			nHolder.exportNotes();
			nHolder.importNotes();
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			break;
		}
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