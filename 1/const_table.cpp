#include "const_table.h"

void ConstTable::addItem(string item)
{
	table.push_back(item);
	itemsCount++;
}

bool ConstTable::readTableFromFile(string fileName)
{
	ifstream file(fileName.c_str());

	if (file.is_open())
	{
		string str;

		while (!file.eof())
		{
			file >> str;
			addItem(str);
		};

		file.close();
		return true;
	}
	else
	{
		file.close();
		return false;
	}
}

int ConstTable::getItemIndexByName(string itemName)
{
	for (int i = 0; i < table.size(); i++)
	{
		if (table[i] == itemName)
		{
			return i;
		}
	}

	return -1;
}

string ConstTable::getItemNameByIndex(int index)
{
	if (index >= 0 && index < table.size())
	{
		return table[index];
	}

	return "Error. Wrong index.";
}

bool ConstTable::isInTheTable(string itemName)
{
	if (getItemIndexByName(itemName) == -1)
		return 0;
	else
		return 1;
}
int ConstTable::size()
{
	return itemsCount;
}