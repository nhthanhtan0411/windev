#pragma once
class CNotesHolder
{
	std::vector<CNote> mNotes;
	std::vector<std::wstring> mDistincTags;
public:
	CNotesHolder();
	~CNotesHolder();

	int importNotes();
	int setNote(int ID, std::vector<std::wstring> Tags, LPWSTR Title, LPWSTR Content);
	int deleteNote(int ID);
	int exportNotes();
	int getTagCount(std::wstring TagName);
	CNote getNote(int ID);
	int getNoteCount();
	std::vector<std::wstring> getDistincTag();
	std::vector<int> getDistincTagCount();
	int addNote(CNote Note);
private: 
	BOOL isExistTag(std::wstring TagName);
	BOOL separateDistincTag(std::vector<std::wstring> Tags);
	int swapWithLastNoteMember(int ID);
	int swapNotes(int srcID, int destID);
	int deleteLastNote();
	int deleteDataFile(int ID);
};

