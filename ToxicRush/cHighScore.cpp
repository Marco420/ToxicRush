#include "cHighScore.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>


cHighScore::cHighScore()
{

}

cHighScore::cHighScore(int maxSize)
{
	this->MAX_SIZE = maxSize;
}

bool cHighScore::loadFromFile(string highScoreFile)
{
	ifstream file;
	string aName;
	int aScore;
	int item = 0;

	file.open(highScoreFile, ios::binary);
	if (!file.is_open())
	{
		cout << "Error opening file " << highScoreFile << endl;
		return false;
	}

	Item tableEntry;
	tableEntries.clear();

	while (!file.eof()) {
		tableEntries.push_back(new Item);
		file >> aName >> aScore;
		tableEntries[item]->Name = aName;
		tableEntries[item]->score = aScore;
		item++;
	}
	file.close();
	this->setTableSize(item);
	return true;
}

bool cHighScore::saveToFile(string highScoreFile)
{
	ofstream file;
	file.open(highScoreFile, ios::binary | ios::out);

	if (!file.is_open())
	{
		cout << "Error opening file " << highScoreFile << " for saving to ..." << endl;
		return false;
	}

	for (int tblEntry = 0; tblEntry < (int)this->tableEntries.size(); tblEntry++)
	{
		file << tableEntries[tblEntry]->Name << " " << tableEntries[tblEntry]->score << endl;
	}

	file.close();
	return true;
}

int  cHighScore::addItem(Item entry)
{
	int row = this->tableEntries.size();

	if (row == this->MAX_SIZE)
	{
		cout << "Table full" << endl;
		this->tableSize = row;
		return row;
	}
	else
	{
		int index = row;
		this->tableEntries.push_back(new Item);
		this->tableEntries[index]->Name = entry.Name;
		this->tableEntries[index]->score = entry.score;
		sort(this->tableEntries.begin(), this->tableEntries.end());
		return this->tableEntries.size();
	}
}

cHighScore::Item cHighScore::getItem(int row)
{
	Item tblItem;
	tblItem.Name = this->tableEntries[row]->Name;
	tblItem.score = this->tableEntries[row]->score;
	return tblItem;
}

void cHighScore::clearTable()
{
	this->tableEntries.clear();
}

string cHighScore::convertToString()
{
	string table = "";
	for (int tblEntry = 0; tblEntry < (int)this->tableEntries.size(); tblEntry++)
	{
		table += tableEntries[tblEntry]->Name + " " + std::to_string(tableEntries[tblEntry]->score) + "\n";
	}
	return table;
}

int cHighScore::getTableSize()
{
	return this->tableSize;
}

void cHighScore::setTableSize(int theSize)
{
	this->tableSize = theSize;
}

cHighScore::~cHighScore()
{
}
