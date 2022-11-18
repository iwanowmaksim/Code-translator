#include "variable_table.h"

bool VariableTable::addItem(string variableName)
{
	integer var(variableName);

	if (correctItemToAdd(var))
	{
		int hash = calculateHash(var.name);
		table[hash].push_back(var);
		itemsCount++;
		return true;
	}
	else
	{
		return false;
	}
}

bool VariableTable::setItemValue(string variableName, int val)
{
	int hash = calculateHash(variableName);
	int itemIndex = findElementInTable(hash, variableName);

	if (itemIndex != -1)
	{
		table[hash][itemIndex].value = val;
		table[hash][itemIndex].isInit = true;
		return true;
	}
	else
	{
		return false;
	}
}

int VariableTable::getValueByName(string variableName)
{
	int hash = calculateHash(variableName);
	int itemIndex = findElementInTable(hash, variableName);

	if (itemIndex != -1)
	{
		if (table[hash][itemIndex].isInit)
		{
			return table[hash][itemIndex].value;
		}
		else
		{
			throwException(string("Variable ") + variableName + string(" not initialized"));
			return 0;
		}
	}
	else
	{
		throwException("Variable " + variableName + " not found");
		return 0;
	}
}

bool VariableTable::isInitItem(string variableName)
{
	int hash = calculateHash(variableName);
	int itemIndex = findElementInTable(hash, variableName);

	if (itemIndex != -1)
	{
		if (table[hash][itemIndex].isInit)
		{
			return 1;
		}
		else
		{
			throwException(string("Variable ") + variableName + string(" not initialized"));
			return 0;
		}
	}
	else
	{
		throwException("Variable " + variableName + " not found");
		return 0;
	}
}

int VariableTable::size()
{
	return itemsCount;
}

bool VariableTable::correctItemToAdd(integer var)
{
	string s(1, var.name[0]);

	if (calculateHash(s) == -1)
	{
		return 0;
	}

	for (int i = 1; i < var.name.length(); i++)
	{
		string str(1, var.name[i]);

		if (calculateHash(str) == -1 && !isdigit(var.name[i]))
		{
			return 0;
		}
	}

	for (auto el : table)
	{
		for (auto el_ : el)
		{
			if (el_.name == var.name)
			{
				return 0;
			}
		}
	}

	return 1;
}

bool VariableTable::correctItemToAssign(integer var)
{
	for (auto el : table)
	{
		for (auto el_ : el)
		{
			if (el_.name == var.name)
			{
				if (el_.isInit == 1)
				{
					return 1;
				}
				else
				{
					return 0;
				}
			}
		}
	}

	return 0;
}

int VariableTable::calculateHash(string str)
{
	if ('a' <= str[0] && str[0] <= 'z')
		return str[0] - 'a';
	if ('A' <= str[0] && str[0] <= 'Z')
		return str[0] - 'A' + 26;
	if (str[0] == '_')
		return 52;

	return -1;
}

int VariableTable::findElementInTable(int hash, string variableName)
{
	if (hash < 0 || hash >= table.size())
	{
		return -1;
	}

	for (int i = 0; i < table[hash].size(); i++)
	{
		if (variableName == table[hash][i].name)
		{
			return i;
		}
	}
	
	return -1;
}

void VariableTable::clearExceptionsFile()
{
	ofstream file("exceptions.txt");
	file.close();
}

void VariableTable::throwException(string exception)
{
	ofstream file("exceptions.txt", ios_base::app);

	file << exception << endl;

	file.close();
}

bool VariableTable::checkItem(string name)
{
	int hash = calculateHash(name);

	if (findElementInTable(hash, name) == -1)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void VariableTable::clear()
{
	table.clear();
}

void VariableTable::resize()
{
	table.resize(MAXIMUM_HASH_NUMBER);
}