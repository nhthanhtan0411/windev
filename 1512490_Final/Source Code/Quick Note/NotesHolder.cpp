#include "stdafx.h"
#include "NotesHolder.h"


CNotesHolder::CNotesHolder()
{
}


CNotesHolder::~CNotesHolder()
{
}

int CNotesHolder::importNotes()
{
	mNotes.clear();
	int ID = 0;
	while (CNote::allDataFileExist(ID)) {
		CNote tmp(ID);
		this->mNotes.push_back(tmp);
		separateDistincTag(tmp.getTags());
		ID++;
	}
	return 1;
}

int CNotesHolder::setNote(int ID, std::vector<std::wstring> Tags, LPWSTR Title, LPWSTR Content)
{
	if (CNote::allDataFileExist(ID)) {
		mNotes[ID].replaceContentInFile(Content);
		mNotes[ID].replaceTagsInFile(Tags);
		mNotes[ID].replaceTitleInFile(Title);
	}
	else {
		mNotes[ID].setTags(Tags);
		mNotes[ID].setTitle(Title);
		mNotes[ID].setContent(Content);
	}

	return 1;
}

int CNotesHolder::deleteNote(int ID)
{
	swapWithLastNoteMember(ID);
	deleteLastNote();
	return 1;
}

int CNotesHolder::exportNotes()
{
	for (int i = 0; i < mNotes.size(); i++) {
		mNotes[i].writeFile();
	}
	return 1;
}

int CNotesHolder::getTagCount(std::wstring TagName)
{
	int count = 0;
	for (int i = 0; i < mNotes.size(); i++) {
			if (mNotes[i].isExistTag(TagName)) {
				count++;
		}
	}
	return count;
}

CNote CNotesHolder::getNote(int ID)
{
	if (mNotes.size() < ID) {
		return NULL;
	}
	return mNotes[ID];
}

int CNotesHolder::getNoteCount()
{
	return mNotes.size();
}

std::vector<std::wstring> CNotesHolder::getDistincTag()
{
	return mDistincTags;
}

std::vector<int> CNotesHolder::getDistincTagCount()
{
	std::vector<int> TagCount;
	for (int i = 0; i < mDistincTags.size(); i++) {
		int tmp;
		tmp = getTagCount(mDistincTags[i]);
		TagCount.push_back(tmp);
	}

	return TagCount;
}

int CNotesHolder::addNote(CNote Note)
{
	try {
		mNotes.push_back(Note);
		return 1;
	}
	catch (int e) { 
		return e;
	}
}

BOOL CNotesHolder::isExistTag(std::wstring TagName)
{
	for (int i = 0; i < mDistincTags.size(); i++) {
		if (mDistincTags[i].compare(TagName) == 0) {
			return true;
		}
	}
	return false;
}

BOOL CNotesHolder::separateDistincTag(std::vector<std::wstring> Tags)
{
	try {
		for (int i = 0; i < Tags.size(); i++) {
			if (isExistTag(Tags[i]) == 0) {
				mDistincTags.push_back(Tags[i]);
			}
		}
		return true;
	}
	catch (int e) {
		return false;
	}
}

int CNotesHolder::swapWithLastNoteMember(int ID)
{
	swapNotes(ID, mNotes.size() - 1);
	return 1;
}

int CNotesHolder::swapNotes(int srcID, int destID)
{

	CNote tmp;
	tmp = mNotes[destID];

	if (CNote::allDataFileExist(srcID) && CNote::allDataFileExist(destID))  {

		mNotes[destID].replaceContentInFile(mNotes[srcID].getContent());
		mNotes[destID].replaceTagsInFile(mNotes[srcID].getTags());
		mNotes[destID].replaceTitleInFile(mNotes[srcID].getTitle());

		mNotes[srcID].replaceContentInFile(tmp.getContent());
		mNotes[srcID].replaceTagsInFile(tmp.getTags());
		mNotes[srcID].replaceTitleInFile(tmp.getTitle());
	}
	else if (!CNote::allDataFileExist(srcID) && !CNote::allDataFileExist(destID)){
		mNotes[destID] = mNotes[srcID];
		mNotes[srcID] = tmp;
	}
	else if (CNote::allDataFileExist(srcID) && !CNote::allDataFileExist(destID)) {
		mNotes[destID] = mNotes[srcID];

		mNotes[srcID].replaceContentInFile(tmp.getContent());
		mNotes[srcID].replaceTagsInFile(tmp.getTags());
		mNotes[srcID].replaceTitleInFile(tmp.getTitle());
	}
	else if (!CNote::allDataFileExist(srcID) && CNote::allDataFileExist(destID)) {

		mNotes[destID].replaceContentInFile(mNotes[srcID].getContent());
		mNotes[destID].replaceTagsInFile(mNotes[srcID].getTags());
		mNotes[destID].replaceTitleInFile(mNotes[srcID].getTitle());

		mNotes[srcID] = tmp;
	}
	else {
		return -1;
	}
	return 1;
}

int CNotesHolder::deleteLastNote()
{
	if (CNote::allDataFileExist(mNotes.size() - 1)) {
		deleteDataFile(mNotes.size() - 1);
		mNotes.pop_back();
	}
	else {
		mNotes.pop_back();
	}
	return 1;
}

int CNotesHolder::deleteDataFile(int ID)
{
	std::string strID = std::to_string(ID);
	std::string cfPath = CONTENTS_FILE_NAME + strID + DATA_EXTENSION_NAME;
	std::string tafPath = TAGS_FILE_NAME + strID + DATA_EXTENSION_NAME;
	std::string tifPath = TITLES_FILE_NAME + strID + DATA_EXTENSION_NAME;

	if (!CNote::allDataFileExist(ID)) {
		return 0;
	}

	DeleteFile((LPCWSTR)cfPath.c_str());
	DeleteFile((LPCWSTR)tafPath.c_str());
	DeleteFile((LPCWSTR)tifPath.c_str());

	return 1;
}