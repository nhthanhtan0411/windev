#pragma once

#include <Windows.h>
#include <iostream>

using namespace std;

const wstring FOLDER_NAME = L".\\DATA_Notes\\";
const wstring FILE_EXTENSION = L".txt";

const wstring TAG_FILE_NAME = FOLDER_NAME + L"Tag" + FILE_EXTENSION;
const wstring NOTE_FILE_NAME = FOLDER_NAME + L"Note" + FILE_EXTENSION;

const wstring TAG = L"Tag";
const wstring NOTE = L"Note";
const wstring ADD = L"Add";
const wstring EDIT = L"Edit";
const wstring RENAME = L"Rename";
const wstring _DELETE = L"Delete";
const wstring STATISTICS = L"Statistics";
const wstring VIEW_NOTE = L"View notes";
const wstring OTHER_NOTES = L"Other notes";
const wstring TOTAL_NOTE = L"Total notes : ";
const wstring TOTAL = L"Total : ";

const wstring TAG_STATISTICS = L"Tag Statistics";
const wstring SELECT_NOTE_TO_EDIT = L"Please select a note to edit";
const wstring SELECT_NOTE_TO_DELETE = L"Please select a note to delete";
const wstring SELECT_TAG_TO_RENAME = L"Please select a tag to rename";
const wstring SELECT_TAG_TO_DELETE = L"Please select a tag to delete";
const wstring NOTE_DELETE_CONFIRM = L"Are you sure to delete this note?";
const wstring TAG_DELETE_CONFIRM = L"Are you sure to delete this tag?";
const wstring TAG_DELETE_ERROR = L"Only remove tag without notes inside";

const wstring NOTIFICATION = L"Notification";
const wstring TAG_INVALID = L"This tag name is invalid, please choose another name";
const wstring TAG_EXIST = L"This tag name already exists, please choose another name";



const int EACH_TAG_MIN_SIZE = 3;

const int CAPTION_EDGE = 20;
const int CAPTION_SPACE = 30;

const COLORREF CHART_COLOR[6] =
{
	RGB(233, 30, 99),
	RGB(156, 39, 176),
	RGB(33, 150, 243),
	RGB(255, 87, 34),
	RGB(76, 175, 80),
	RGB(121, 85, 72)
};