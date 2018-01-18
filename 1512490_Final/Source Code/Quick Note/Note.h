#pragma once
class CNote
{
	static int mNoteCount;
	std::vector<std::wstring>  mTags; //Dynamic variable to store array of tags
	std::wstring mTitle;
	std::wstring mContent;
	int mID;
public:
	CNote();
	CNote(int ID);
	CNote(std::vector<std::wstring> Tags, std::wstring Title, std::wstring Content);
	~CNote();

	std::vector<std::wstring> setTags(std::vector<std::wstring> TagsArray);
	std::wstring setTitle(std::wstring Title);
	std::wstring setContent(std::wstring Content);
	int setID(int ID);

	std::vector<std::wstring> getTags() { return mTags; };
	std::wstring getTitle() { return mTitle; };
	std::wstring getContent() { return mContent; };
	int getID(){return mID;};

	int writeFile();
	int deleteNoteInFile(); //Not implemented

	int replaceContentInFile(std::wstring Content);
	int replaceTagsInFile(std::vector<std::wstring>  Tags);
	int replaceTitleInFile(std::wstring Title);

	int loadDataFromFile(int ID);

	static BOOL allDataFileExist(int ID);
	BOOL isExistTag(std::wstring TagName);
private:
	int writeContent(std::string fPath);
	int writeTags(std::string fPath);
	int writeTitle(std::string fPath);
	int loadTitle(int ID, std::string fPath);
	int loadContent(int ID, std::string fPath);
	int loadTags(int ID, std::string fPath);
};

BOOL isExist(const std::string& FileName);