#pragma once

#include "Constant.h"
#include "Library.h"

struct Note
{
	wstring NoteID; // also filename
	wstring Tag;
	wstring Data;
};

class CNotes
{
private:
	vector<Note> myList;

	void readCatalogue();
	void writeCatalogue();
public:
	
	void readData();
	void writeData();

	void addNote(wstring NoteID, wstring strTag, wstring strData);
	void deleteNote(wstring NoteID);
	void editNote(wstring NoteID, wstring strTag, wstring strData);

	wstring getNoteID(int position);
	int getNotePosition(wstring NoteID);

	int size();
	Note get(int position);

	CNotes();
	~CNotes();
};

