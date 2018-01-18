#include "stdafx.h"
#include "Note.h"

int CNote::mNoteCount = 0;

CNote::CNote()
{
	mID = mNoteCount;
	mNoteCount++;
}

CNote::CNote(int ID)
{
	loadDataFromFile(ID);
}

CNote::CNote(std::vector<std::wstring> Tags, std::wstring Title, std::wstring Content)
{
	mContent = Content;
	mTags = Tags;
	mTitle = Title;
	mNoteCount++;
	mID = mNoteCount;
}


CNote::~CNote()
{
	mNoteCount--;
}

std::vector<std::wstring> CNote::setTags(std::vector<std::wstring>  TagsArray)
{
	mTags = TagsArray;
	return mTags;
}

std::wstring CNote::setTitle(std::wstring Title)
{
	mTitle = Title;
	return mTitle;
}

std::wstring CNote::setContent(std::wstring Content)
{
	mContent = Content;
	return mContent;
}

int CNote::setID(int ID)
{
	mID = ID;
	return mID;
}

int CNote::writeFile()
{
	try {

		writeContent(FILE_PATH);
		writeTags(FILE_PATH);
		writeTitle(FILE_PATH);

		return 1;
	}
	catch (int e) {
		return e;
	}
}

int CNote::deleteNoteInFile()
{
	return 0;
}

int CNote::replaceContentInFile(std::wstring Content)
{
	/*if (mID == NULL)
		return 0;*/

	std::wofstream fout;
	std::string strID = std::to_string(this->mID);
	std::string fPath = CONTENTS_FILE_NAME + strID;
	fPath = fPath + DATA_EXTENSION_NAME;
	fout.open(fPath, std::ios::out);

	mContent = Content;
	fout << Content;

	fout.close();
	return 1;
}

int CNote::replaceTagsInFile(std::vector<std::wstring> Tags)
{
	/*if (mID == NULL)
		return 0;*/

	std::wofstream fout;
	std::string strID = std::to_string(this->mID);
	std::string fPath = TAGS_FILE_NAME + strID + DATA_EXTENSION_NAME;
	fout.open(fPath, std::ios::out);

	mTags = Tags;

	for (int i = 0; i < mTags.size(); i++) {
		fout << mTags[i] << std::endl;
	}

	fout.close();
	return 1;
}

int CNote::replaceTitleInFile(std::wstring Title)
{
	/*if (mID == NULL)
		return 0;*/

	std::wofstream fout;
	std::string strID = std::to_string(this->mID);
	std::string fPath = TITLES_FILE_NAME + strID + DATA_EXTENSION_NAME;
	fout.open(fPath, std::ios::out);

	mTitle = Title;
	fout << mTitle << std::endl;

	fout.close();
	return 1;
}

int CNote::loadDataFromFile(int ID)
{
	std::string strID = std::to_string(ID);
	std::string cfPath = CONTENTS_FILE_NAME + strID + DATA_EXTENSION_NAME;
	std::string tafPath = TAGS_FILE_NAME + strID + DATA_EXTENSION_NAME;
	std::string tifPath = TITLES_FILE_NAME + strID + DATA_EXTENSION_NAME;

	if (!allDataFileExist(ID)) {
		return 0;
	}
	loadTitle(ID, tifPath);
	loadContent(ID, cfPath);
	loadTags(ID, tafPath);

	mID = ID;
	return 1;
}

BOOL CNote::allDataFileExist(int ID)
{
	std::string strID = std::to_string(ID);
	std::string cfPath = CONTENTS_FILE_NAME + strID + DATA_EXTENSION_NAME;
	std::string tafPath = TAGS_FILE_NAME + strID + DATA_EXTENSION_NAME;
	std::string tifPath = TITLES_FILE_NAME + strID + DATA_EXTENSION_NAME;

	if (!isExist(cfPath)) {
		return false;
	}

	if (!isExist(tafPath)) {
		return false;
	}

	if (!isExist(tifPath)) {
		return false;
	}

	return true;
}

BOOL CNote::isExistTag(std::wstring TagName)
{
	for (int i = 0; i < mTags.size(); i++) {
		if (TagName.compare(mTags[i]) == 0) {
			return true;
		}
	}
	return false;
}

int CNote::writeContent(std::string fPath)
{
	/*if (mID == NULL)
		return 0;*/

	std::wofstream fout;

	std::string ID = std::to_string(this->mID);
	std::string cfPath = fPath + CONTENTS_FILE_NAME + ID + DATA_EXTENSION_NAME;
	fout.open(cfPath, std::ios::out);

	const std::locale utf8_locale = std::locale(std::locale(),
		new std::codecvt_utf8<wchar_t>());
	fout.imbue(utf8_locale);

	fout << mContent;

	fout.close();
	return 1;
}

int CNote::writeTags(std::string fPath)
{
	/*if (mID == NULL)
		return 0;*/

	std::wofstream fout;
	std::string ID = std::to_string(this->mID);
	std::string tafPath = fPath + TAGS_FILE_NAME + ID + DATA_EXTENSION_NAME;
	fout.open(tafPath, std::ios::out);
	
	const std::locale utf8_locale = std::locale(std::locale(),
		new std::codecvt_utf8<wchar_t>());
	fout.imbue(utf8_locale);

	for (int i = 0; i < mTags.size(); i++) {
		fout << mTags[i] << std::endl;
	}

	fout.close();
	return 1;
}

int CNote::writeTitle(std::string fPath)
{
	/*if (mID == NULL)
		return 0; */

	std::wofstream fout;
	std::string ID = std::to_string(this->mID);
	std::string tifPath = fPath + TITLES_FILE_NAME + ID + DATA_EXTENSION_NAME;
	fout.open(tifPath, std::ios::out);

	const std::locale utf8_locale = std::locale(std::locale(),
		new std::codecvt_utf8<wchar_t>());
	fout.imbue(utf8_locale);

	fout << mTitle << std::endl;

	fout.close();
	return 1;
}

int CNote::loadTitle(int ID, std::string fPath)
{
	std::wifstream fin;
	fin.open(fPath, std::ios::in);

	const std::locale utf8_locale = std::locale(std::locale(),
		new std::codecvt_utf8<wchar_t>());
	fin.imbue(utf8_locale);

	std::wstring tmpTitle;

	//fin.getline(mTitle, MAX_LINE_BUFFER);
	std::getline(fin, tmpTitle);

	mTitle = const_cast <LPWSTR> (tmpTitle.c_str());

	return 1;
}

int CNote::loadContent(int ID, std::string fPath)
{
	std::wifstream fin;
	fin.open(fPath, std::ios::in);

	const std::locale utf8_locale = std::locale(std::locale(),
		new std::codecvt_utf8<wchar_t>());
	fin.imbue(utf8_locale);

	mContent = L"";
	std::wstring tmpContent = L"";
	while (!fin.eof()) {
		std::wstring wtmp;
		std::getline(fin, wtmp);
		
		tmpContent = tmpContent + wtmp;
	}
	mContent = const_cast<LPWSTR>(tmpContent.c_str());
	return 1;
}

int CNote::loadTags(int ID, std::string fPath)
{
	std::wifstream fin;
	fin.open(fPath, std::ios::in);

	const std::locale utf8_locale = std::locale(std::locale(),
		new std::codecvt_utf8<wchar_t>());
	fin.imbue(utf8_locale);

	while (!fin.eof()) {

		std::wstring wtmp;
		std::getline(fin, wtmp);
		wtmp.erase(std::remove(wtmp.begin(), wtmp.end(), '\n'), wtmp.end());
		//lptmp = const_cast<LPWSTR>(wtmp.c_str());
		if (L"" == wtmp) {
			//do nothing
		}
		else {

			mTags.push_back(wtmp);
		}

	}
	return 1;
}

BOOL isExist(const std::string& FileName) {
	struct stat buffer;
	return (stat(FileName.c_str(), &buffer) == 0);
}