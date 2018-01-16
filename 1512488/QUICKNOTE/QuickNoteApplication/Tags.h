#pragma once

#include "Notes.h"

using namespace std;

struct Tag
{
	wstring Name;
	vector<int> NoteIDs;
};

class CTags
{
private:
	vector<Tag> myList;

public:

	void readData();
	void writeData();

	void addNote(int NoteID, wstring strTag);
	void deleteNote(int NoteID);
	void editNote(int NoteID, wstring strTag);

	vector<int> getNoteIDByTag(int TagPos);

	wstring getTagByNoteID(int NoteID);

	// check is empty to delete
	bool isDelete(int position);

	// only when this tag is empty note
	void deleteTag(int position);

	// 
 	void renameTag(int position, wstring newName);

	// check exist before rename
	bool isExistTagName(wstring name);

	// to lower 
	wstring _toLower(wstring str)
	{
		std::locale::global(std::locale(""));  // (*)
		std::wcout.imbue(std::locale());
		auto& f = std::use_facet<std::ctype<wchar_t>>(std::locale());

		f.toupper(&str[0], &str[0] + str.size());

		return str;
	}


	// for auto complete textbox
	wstring autoTag(wstring strTag);

	// find 4 largest data
	// 5 is the remaining ones
	int getDataForChart(vector<wstring> &data, vector<int>&value);

	int size();

	wstring getName(int position);

	CTags();
	~CTags();
};