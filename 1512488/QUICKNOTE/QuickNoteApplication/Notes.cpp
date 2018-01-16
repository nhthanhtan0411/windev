#include "stdafx.h"
#include "Notes.h"

void CNotes::readCatalogue()
{
	wstring str;
	wifstream input;
	int n;
	Note tempNote;

	// open file
	input.open(NOTE_FILE_NAME, ios::in);

	// file doesn't exist
	if (!input)
	{
		return;
	}

	// constraint
	input.imbue(locale(input.getloc(), new codecvt_utf8_utf16<wchar_t>));

	// note's count
	input >> n;

	// get the line after number n
	getline(input, str);

	for (int i = 0; i < n; i++) {
		// get name
		getline(input, str);

		// 
		tempNote.NoteID = str;

		// insert to main list
		myList.push_back(tempNote);
	}

	input.close();
}

void CNotes::writeCatalogue()
{
	wstring str;
	wofstream output;

	// open file
	output.open(NOTE_FILE_NAME, ios::out);

	// constraint
	output.imbue(locale(output.getloc(), new codecvt_utf8_utf16<wchar_t>));

	// make a string and write to file
	str = to_wstring(myList.size()) + L"\n";
	output.write(str.c_str(), str.length());

	int size = myList.size();
	for (int i = 0; i < size; i++)
	{
		str = myList[i].NoteID + L"\n";
		output.write(str.c_str(), str.length());
	}

	//
	output.close();
}

void CNotes::readData()
{
	readCatalogue();

	int size = myList.size();
	Note tempNote;

	wstring str, data;
	wifstream input;

	for (int i = 0; i < size; i++)
	{
		tempNote = myList[i];

		wstring name = FOLDER_NAME + tempNote.NoteID + FILE_EXTENSION;

		// open file
		input.open(name, ios::in);

		// file doesn't exist
		if (!input)
		{
			return;
		}

		// constraint
		input.imbue(locale(input.getloc(), new codecvt_utf8_utf16<wchar_t>));

		// get tag 
		getline(input, str);
		tempNote.Tag = str;

		// get data
		data = L"";
		while (!input.eof())
		{
			getline(input, str);
			data += (str + L"\n");
		}

		input.close();
		tempNote.Data = data;
		myList[i] = tempNote;
	}
}

void CNotes::writeData()
{
	writeCatalogue();

	Note tempNote;
	int size = myList.size();
	wstring str;
	wofstream output;
	for (int i = 0; i < size; i++)
	{
		tempNote = myList[i];

		wstring name = FOLDER_NAME + tempNote.NoteID + FILE_EXTENSION;

		// open file
		output.open(name, ios::out);

		// constraint
		output.imbue(locale(output.getloc(), new codecvt_utf8_utf16<wchar_t>));

		// make a string and write to file
		str = tempNote.Tag + L"\n";
		output.write(str.c_str(), str.length());

		str = tempNote.Data + L"\n";
		output.write(str.c_str(), str.length());

		output.close();
	}
}

void CNotes::addNote(wstring NoteID, wstring strTag, wstring strData)
{
	Note tempNote;

	tempNote.NoteID = NoteID;
	tempNote.Tag = strTag;
	tempNote.Data = strData;

	// add to list
	myList.push_back(tempNote);

	// add to file system
	wstring str;
	wofstream output;
	wstring name = FOLDER_NAME + tempNote.NoteID + FILE_EXTENSION;

	// open file
	output.open(name, ios::out);

	// constraint
	output.imbue(locale(output.getloc(), new codecvt_utf8_utf16<wchar_t>));

	// make a string and write to file
	str = tempNote.Tag + L"\n";
	output.write(str.c_str(), str.length());

	str = tempNote.Data + L"\n";
	output.write(str.c_str(), str.length());

	output.close();
}

void CNotes::deleteNote(wstring NoteID)
{
	int size = myList.size();
	for (int i = 0; i < size; i++)
	{
		if (myList[i].NoteID == NoteID)
		{
			// remove from list
			myList.erase(myList.begin() + i);

			// remove from file system
			wstring name = FOLDER_NAME + NoteID + FILE_EXTENSION;
			_wremove(name.c_str());

			return;
		}
	}
}

void CNotes::editNote(wstring NoteID, wstring strTag, wstring strData)
{
	// delete then add new
	deleteNote(NoteID);
	addNote(NoteID, strTag, strData);
}

wstring CNotes::getNoteID(int position)
{
	if (position < myList.size())
	{
		return myList[position].NoteID;
	}

	return wstring();
}

int CNotes::getNotePosition(wstring NoteID)
{
	int size = myList.size();

	for (int i = 0; i < size; i++)
	{
		if (myList[i].NoteID == NoteID)
		{
			return i;
		}
	}

	return -1;
}


int CNotes::size()
{
	return myList.size();
}

Note CNotes::get(int position)
{
	return myList[position];
}

CNotes::CNotes()
{
	myList.clear();
}


CNotes::~CNotes()
{
	myList.clear();
}
