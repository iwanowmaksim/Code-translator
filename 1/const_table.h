#pragma once

#include "scaner.h"

class ConstTable 
{
public:
	ConstTable() {}

	ConstTable(string fileName) 
	{
		readTableFromFile(fileName);
	}
	~ConstTable() 
	{
		table.clear();
	}

	void addItem(string item);
	bool readTableFromFile(string fileName);
	int getItemIndexByName(string itemName);
	string getItemNameByIndex(int index);
	bool isInTheTable(string itemName);
	int size();

private:
	int itemsCount = 0;
	vector<string> table;
};