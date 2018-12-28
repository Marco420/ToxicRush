#pragma once
#include <string>
#include <vector>

using namespace std;

class cHighScoreTable
{
private:
	struct Item {
		string Name;
		int score;

		bool operator < ( const Item& rhs) const
		{
			return rhs.score < score;
		}
	};

	int tableSize;
	int MAX_SIZE = 5;

	Item tableEntry;

	vector<Item*> tableEntries;
	vector<Item*>::iterator row;


public:
	cHighScoreTable();
	cHighScoreTable(int maxSize);

	bool loadFromFile(string highScoreFile);
	bool saveToFile(string highScoreFile);
	void modifyItem(string name,int score);
	int  addItem(Item entry);
	Item getItem(int row);
	void clearTable();
	string convertToString();
	int getTableSize();
	void setTableSize(int theSize);

	virtual ~cHighScoreTable();
};

