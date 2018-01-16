#include "stdafx.h"
#include "Tags.h"

void CTags::readData()
{
	wstring str;
	wifstream input;
	int n, m, temp;
	Tag tempTag;

	// open file
	input.open(TAG_FILE_NAME, ios::in);

	// file doesn't exist
	if (!input)
	{
		return;
	}

	// constraint
	input.imbue(locale(input.getloc(), new codecvt_utf8_utf16<wchar_t>));

	// tag's count
	input >> n;

	// get the line after number n
	getline(input, str);

	for (int i = 0; i < n; i++) {
		// get tag name
		getline(input, str);
		tempTag.Name = str;

		// get note id count
		input >> m;

		// clear before insert (because it is a global variable)
		tempTag.NoteIDs.clear();

		// get all note id
		for (int j = 0; j < m; j++)
		{
			input >> temp;
			tempTag.NoteIDs.push_back(temp);
		}

		// get the line after final number
		getline(input, str);

		// insert to main list
		myList.push_back(tempTag);
	}


	input.close();
}

void CTags::writeData()
{
	wofstream output;
	wstring temp;
	int size = myList.size();

	output.open(TAG_FILE_NAME, ios::out);

	// constraint
	output.imbue(locale(output.getloc(), new codecvt_utf8_utf16<wchar_t>));

	temp = to_wstring(size) + L"\n";
	output.write(temp.c_str(), temp.length());

	for (int i = 0; i < size; i++)
	{
		temp = myList[i].Name + L"\n";
		output.write(temp.c_str(), temp.length());

		temp = to_wstring(myList[i].NoteIDs.size()) + L"\n";
		output.write(temp.c_str(), temp.length());

		temp = L"";
		for (int j = 0; j < myList[i].NoteIDs.size(); j++)
		{
			temp = temp + to_wstring(myList[i].NoteIDs[j]) + L" ";
		}

		temp = temp + L"\n";
		output.write(temp.c_str(), temp.length());
	}

	output.close();
}

void CTags::addNote(int NoteID, wstring strTag)
{
	int pos1, pos2;
	int size, list_size;
	int countComma = 0;
	wstring temp;

	// store cutted string
	vector<wstring> result;

	// count the number of commas
	pos1 = -1;
	while (true)
	{
		pos2 = strTag.find(L",", pos1 + 1);
		if (pos2 != wstring::npos)
		{
			countComma++;
			pos1 = pos2;
		}
		else
		{
			break;
		}
	}

	// cut and put into vector
	pos1 = -1;
	while (countComma-- >= 0)
	{
		pos2 = strTag.find(L",", pos1 + 1);

		temp = strTag.substr(pos1 + 1, pos2 - pos1 - 1);

		result.push_back(temp);

		pos1 = pos2;
	}

	// remove string have only ' '
	for (int i = 0; i < result.size(); i++)
	{
		bool remove = true;
		for (int j = 0; j < result[i].length(); j++)
		{
			if (result[i][j] != L' ')
			{
				remove = false;
				break;
			}
		}

		if (remove)
		{
			result.erase(result.begin() + i);
			i--;
		}
	}

	// remove string have size are smaller than 3
	for (int i = 0; i < result.size(); i++)
	{
		if (result[i].length() <= EACH_TAG_MIN_SIZE)
		{
			result.erase(result.begin() + i);
		}
	}

	// remove ' ' before and after each string
	for (int i = 0; i < result.size(); i++)
	{
		wstring temp = result[i];

		while (temp[0] == L' ')
		{
			temp.erase(0, 1);
		}

		while (temp[temp.length() - 1] == L' ')
		{
			temp.erase(temp.length() - 1, 1);
		}

		result[i] = temp;
	}

	for (int i = 0; i < result.size()-1; i++)
	{
		for (int j = i + 1; j < result.size(); j++)
		{
			if (result[i] == result[j])
			{
				result.erase(result.begin() + j);
				--j;
			}
		}
	}

	// compare with myList and solve data
	size = myList.size();
	for (int i = 0; i < result.size(); i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (result[i] == myList[j].Name)
			{
				vector<int> temp(myList[j].NoteIDs);
				temp.push_back(NoteID);
				myList[j].NoteIDs = temp;
				result.erase(result.begin() + i);
				i--;
				break;
			}
		}
	}

	// if there which is not in the list, then insert
	for (int i = 0; i < result.size(); i++)
	{
		Tag tempTag;

		tempTag.Name = result[i];
		tempTag.NoteIDs.push_back(NoteID);

		myList.push_back(tempTag);
	}
}

void CTags::deleteNote(int NoteID)
{
	int size = myList.size();
	int list_size;
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < myList[i].NoteIDs.size(); j++)
		{
			if (myList[i].NoteIDs[j] == NoteID)
			{
				myList[i].NoteIDs.erase(myList[i].NoteIDs.begin() + j);
				break;
			}
		}
	}
}

void CTags::editNote(int NoteID, wstring strTag)
{
	deleteNote(NoteID);
	addNote(NoteID, strTag);
}

vector<int> CTags::getNoteIDByTag(int TagPos)
{
	return myList[TagPos].NoteIDs;
}

wstring CTags::getTagByNoteID(int NoteID)
{
	wstring result = L"";
	int size = myList.size();
	int list_size;
	Tag temp;

	for (int i = 0; i < size; i++)
	{
		temp = myList[i];
		list_size = temp.NoteIDs.size();
		for (int j = 0; j < list_size; j++)
		{
			if (temp.NoteIDs[j] == NoteID)
			{
				result = result + (temp.Name + L", ");
				break;
			}
		}

	}

	return result.substr(0, result.size() - 2);
}

bool CTags::isDelete(int position)
{
	if (position < myList.size())
	{
		if (myList[position].NoteIDs.size() == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

void CTags::deleteTag(int position)
{
	if (position < myList.size())
	{
		myList.erase(myList.begin() + position);
	}
}

void CTags::renameTag(int postion, wstring newName)
{
	if (postion < myList.size())
	{
		myList[postion].Name = newName;
	}
}

bool CTags::isExistTagName(wstring name)
{
	int size = myList.size();
	for (int i = 0; i < size; i++)
	{
		if (myList[i].Name == name)
		{
			return true;
		}
	}

	return false;
}

wstring CTags::autoTag(wstring strTag)
{
	wstring result, token;
	int pos,size,len;
	bool isOK;
	vector<Tag> list;

	// 
	result = strTag;

	if (result == L"")
	{
		return strTag;
	}

	// find ,
	pos = strTag.find_last_of(L",");

	// not found ,
	if (pos == wstring::npos)
	{
		token = strTag;
	}
	else
	{
		token = strTag.substr(pos + 1, strTag.length() - pos - 1);
	}

	// return if empty
	if (token == L"")
	{
		return strTag;
	}

	// check if all char is ' ' 
	isOK = false;
	for (int i = 0; i < token.length(); i++)
	{
		if (token[i] == L' ')
		{
			
		}
		else
		{
			isOK = true;
		}
	}

	// if all char is ' '  return 
	if (!isOK)
	{
		return strTag;
	}

	// clear first ' '
	while (token[0] == L' ')
	{
		token.erase(0, 1);
	}

	// search
	size = myList.size();
	len = token.length();
	for (int i = 0; i < size; i++)
	{
		if (_toLower(myList[i].Name.substr(0, len)) == _toLower(token))
		{
			list.push_back(myList[i]);
		}
	}

	// 
	size = list.size();
	if (size == 0)
	{
		return strTag;
	}

	// sort
	for (int i = 0; i < size-1; i++)
	{
		for (int j = i + 1; j < size; j++)
		{
			if (list[i].NoteIDs.size() < list[j].NoteIDs.size())
			{
				swap(list[i], list[j]);
			}
		}
	}

	// 
	if (pos == wstring::npos)
	{
		result = L"";
	}
	else
	{
		result = result.substr(0, pos+1);
	}
	
	result = result + list[0].Name;

	return result;
}

int CTags::getDataForChart(vector<wstring>& data, vector<int>& value)
{
	int size = myList.size();
	int sum = 0;

	data.resize(5);
	value.resize(5);

	for (int i = 0; i < size; i++)
	{
		sum += myList[i].NoteIDs.size();
	}


	for (int i = 0; i < 5; i++)
	{
		data[i] = L"";
		value[i] = 0;
	}

	if (size <= 4)
	{
		for (int i = 0; i < size; i++)
		{
			data[i] = myList[i].Name;
			value[i] = myList[i].NoteIDs.size();

			sum -= value[i];
		}

		data[size] = OTHER_NOTES;
		value[size] = sum;

		return size;
	}

	vector<Tag> sort_array;

	for (int i = 0; i < size; i++)
	{
		sort_array.push_back(myList[i]);
	}

	Tag temp;
	for (int i = 0; i < size - 1; i++)
	{
		for (int j = i + 1; j < size; j++)
		{
			if (sort_array[i].NoteIDs.size() < sort_array[j].NoteIDs.size())
			{
				temp = sort_array[i];
				sort_array[i] = sort_array[j];
				sort_array[j] = temp;
			}
		}
	}

	for (int i = 0; i < 4; i++)
	{
		data[i] = sort_array[i].Name;
		value[i] = sort_array[i].NoteIDs.size();
		sum -= value[i];
	}

	data[4] = OTHER_NOTES;
	value[4] = sum;

	return size;
}

int CTags::size()
{
	return myList.size();
}

wstring CTags::getName(int position)
{
	if (position < myList.size())
	{
		return myList[position].Name;
	}
	else
	{
		return L"";
	}
}

CTags::CTags()
{
	myList.clear();
}

CTags::~CTags()
{
	myList.clear();
}
