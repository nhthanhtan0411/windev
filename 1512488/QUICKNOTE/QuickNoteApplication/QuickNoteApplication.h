#pragma once

#include "resource.h"
#include "Tags.h"

//#include "ShortcutKeyDLL.cpp"

using namespace std;

#define MAX_LOADSTRING 100
#define	WM_USER_SHELLICON WM_USER + 1

// Global Variables:
HINSTANCE	hInst;                                // current instance
WCHAR		szTitle[MAX_LOADSTRING];			  // The title bar text
WCHAR		szWindowClass[MAX_LOADSTRING];        // the main window class name

												  // Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Note(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Rename(HWND, UINT, WPARAM, LPARAM);

// ---------------------
void	OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void	OnPaint(HWND hwnd);
void	OnDestroy(HWND hwnd);
BOOL	OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
LRESULT OnNotify(HWND hwnd, int idFrom, NMHDR *pnm);


HINSTANCE	hinstLib;

HWND	treeTag;
HWND	listNote;
HWND	txtNoteCount;
HWND	btnTagRename;
HWND	btnTagDelete;
HWND	btnNoteAdd;
HWND	btnNoteEdit;
HWND	btnNoteDelete;
HWND	btnStatistics;
HWND	groupTag;
HWND	groupNote;

HWND	dialogAddNote;

vector<HWND> chartHWND;

CTags	myTags;
CNotes	myNotes;

bool isShow = true;
bool isStatistics = false;

// because the dialog box to add notes and edit notes is a common dialog box.
// so this flag is used to distinguish
bool isEditNote = false;

// --------------------------- for get selected ---------------------------
int getSelectedNote()
{
	int selected = ListView_GetSelectionMark(listNote);
	if (selected == -1)
	{
		return -1;
	}

	LVITEM lv;
	TCHAR *buffer = new TCHAR[256];
	lv.mask = LVIF_PARAM;
	lv.iItem = selected;
	lv.iSubItem = 0;
	lv.pszText = buffer;
	lv.cchTextMax = 256;
	ListView_GetItem(listNote, &lv);
	return lv.lParam;
}

int getSelectedTag()
{
	HTREEITEM hTreeItem = TreeView_GetSelection(treeTag);
	if (hTreeItem == NULL)
	{
		return -1;
	}

	TVITEMEX tv;
	TCHAR *buffer = new TCHAR[256];
	tv.mask = TVIF_TEXT;
	tv.hItem = hTreeItem;
	tv.pszText = buffer;
	tv.cchTextMax = 256;

	TreeView_GetItem(treeTag, &tv);

	return tv.lParam;
}

// --------------------------- for update GUI ---------------------------
void updateTreeTag()
{
	TreeView_DeleteAllItems(treeTag);

	TV_INSERTSTRUCT tvInsert;
	tvInsert.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;

	tvInsert.hParent = NULL;
	tvInsert.hInsertAfter = TVI_ROOT;
	tvInsert.item.iImage = 3;
	tvInsert.item.iSelectedImage = 3;
	tvInsert.item.pszText = _T("All Notes");
	tvInsert.item.lParam = 0;		// index
	HTREEITEM itemAllNote = TreeView_InsertItem(treeTag, &tvInsert);

	tvInsert.item.pszText = _T("Tags");
	tvInsert.item.lParam = 1;		// index
	HTREEITEM itemTags = TreeView_InsertItem(treeTag, &tvInsert);

	// parent is Tags
	tvInsert.hParent = itemTags;
	tvInsert.item.iImage = 0;
	tvInsert.item.iSelectedImage = 1;

	int size = myTags.size();
	for (int i = 0; i < size; i++)
	{
		// convert wstring to LPWSTR
		wstring a = myTags.getName(i);
		vector<wchar_t> buf(a.begin(), a.end());
		buf.push_back(0);

		tvInsert.item.lParam = i + 2;		// index
		tvInsert.item.pszText = buf.data();
		TreeView_InsertItem(treeTag, &tvInsert);
	}

	TreeView_Expand(treeTag, itemTags, TVE_EXPAND);
	TreeView_SelectItem(treeTag, itemAllNote);

	SetFocus(treeTag);
}

void updateListNote()
{
	ListView_DeleteAllItems(listNote);

	LV_ITEM lvItem;
	lvItem.mask = LVIF_TEXT | LVIF_PARAM;
	lvItem.iSubItem = 0;

	int size = myNotes.size();
	for (int i = 0; i < size; i++)
	{
		lvItem.iItem = i;
		lvItem.lParam = i;

		// convert wstring to LPWSTR
		wstring a = myNotes.get(i).Data;
		vector<wchar_t> buf(a.begin(), a.end());
		buf.push_back(0);

		lvItem.pszText = buf.data();

		ListView_InsertItem(listNote, &lvItem);
	}


	// convert wstring to LPWSTR
	wstring a = TOTAL_NOTE + to_wstring(size);
	vector<wchar_t> buf(a.begin(), a.end());
	buf.push_back(0);
	SetWindowText(txtNoteCount, buf.data());
}

void updateListNote(int TagID)
{
	ListView_DeleteAllItems(listNote);

	LV_ITEM lvItem;
	lvItem.mask = LVIF_TEXT | LVIF_PARAM;
	lvItem.iSubItem = 0;

	vector<int> NoteIDs = myTags.getNoteIDByTag(TagID);

	int size = NoteIDs.size();
	for (int i = 0; i < size; i++)
	{
		lvItem.iItem = i;
		lvItem.lParam = NoteIDs[i] - 1;

		// convert wstring to LPWSTR
		wstring a = myNotes.get(NoteIDs[i] - 1).Data;// -1
		vector<wchar_t> buf(a.begin(), a.end());
		buf.push_back(0);

		lvItem.pszText = buf.data();

		ListView_InsertItem(listNote, &lvItem);
	}

	// convert wstring to LPWSTR
	wstring a = TOTAL_NOTE + to_wstring(size);
	vector<wchar_t> buf(a.begin(), a.end());
	buf.push_back(0);
	SetWindowText(txtNoteCount, buf.data());
}

// --------------------------- for noti ---------------------------
NOTIFYICONDATA nidApp = {

};


// --------------------------- for hook keyboard ---------------------------

// hook keyboard
HHOOK hHook = NULL;

void doInstallHook(HWND hWnd)
{
	typedef bool (__cdecl *MYPROC)(HWND);
	HINSTANCE hinstLib;
	MYPROC ProcAddr;

	hinstLib = LoadLibrary(L"ShortcutKeyDLL.dll");

	if (hinstLib != NULL)
	{
		MessageBox(0, L"Installed Hook", 0, 0);
		ProcAddr = (MYPROC)GetProcAddress(hinstLib, "_doInstallHook");
		MessageBox(0, (LPCWSTR) ProcAddr, L"NULL", 0);
		if (ProcAddr != NULL)
		{
			MessageBox(0, L"Installed Hook", 0, 0);
			ProcAddr(hWnd);
			
		}
	}
}

void doRemoveHook(HWND hWnd)
{
	typedef void(*MYPROC)(HWND);
	HINSTANCE hinstLib;
	MYPROC ProcAddr;

	hinstLib = LoadLibrary(L"ShortcutKeyDLL.dll");

	if (hinstLib != NULL)
	{
		ProcAddr = (MYPROC)GetProcAddress(hinstLib, "_doRemoveHook");
		if (ProcAddr != NULL)
		{
			ProcAddr(hWnd);
		}
	}
}

void prepareNoteDialog()
{
	HWND hTag = GetDlgItem(dialogAddNote, IDC_EDIT_TAG);
	HWND hData = GetDlgItem(dialogAddNote, IDC_EDIT_DATA);

	if (isEditNote)
	{
		int selectedNote = getSelectedNote();

		// convert wstring to LPWSTR
		wstring a = myTags.getTagByNoteID(selectedNote + 1);
		vector<wchar_t> buf(a.begin(), a.end());
		buf.push_back(0);
		SetWindowText(hTag, buf.data());

		a = myNotes.get(selectedNote).Data;
		vector<wchar_t> buf_(a.begin(), a.end());
		buf_.push_back(0);
		SetWindowText(hData, buf_.data());

		SetFocus(hData);
	}
	else
	{
		SetWindowText(hTag, L"");
		SetWindowText(hData, L"");
	}
}

// --------------------------- for auto complete edit text ---------------------------
WNDPROC oldEditProc;

void setData()
{
	HWND hDlg = dialogAddNote;

	HWND hTag = GetDlgItem(hDlg, IDC_EDIT_TAG);
	HWND hTagAuto = GetDlgItem(hDlg, IDC_TEXT_TAG);

	int size = GetWindowTextLength(hTagAuto);
	WCHAR *bufferTag = new WCHAR[size + 1];
	GetWindowText(hTagAuto, bufferTag, size + 1);

	SetWindowText(hTag, bufferTag);
	SendMessage(hTag, EM_SETSEL, size, size);
}

void setFocus()
{
	HWND hDlg = dialogAddNote;

	HWND hData = GetDlgItem(hDlg, IDC_EDIT_DATA);

	SetFocus(hData);
}

LRESULT CALLBACK subEditProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_DOWN:
			setData();
			break;

		case VK_TAB:
			setFocus();
			break;
		}
	default:
		return CallWindowProc(oldEditProc, hWnd, message, wParam, lParam);
	}
	return 0;
}