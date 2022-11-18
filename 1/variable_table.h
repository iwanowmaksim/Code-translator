#pragma once

#include "const_table.h"

const int MAXIMUM_HASH_NUMBER = 53; //a-z, A-Z, _

class VariableTable 
{
public:
	VariableTable() 
	{
		table.resize(MAXIMUM_HASH_NUMBER);
		clearExceptionsFile();
	}
	
	~VariableTable() 
	{
		clear();
	}

	bool addItem(string variableName);
	bool setItemValue(string variableName, int val);
	int getValueByName(string variableName);
	bool isInitItem(string variableName);
	int size();
	bool checkItem(string name);
	void clear();
	void resize();

private:
	int itemsCount = 0;
	vector<vector<integer>> table;

	bool correctItemToAdd(integer var);
	bool correctItemToAssign(integer var);
	int calculateHash(string str);
	int findElementInTable(int hash, string variableName);
	void clearExceptionsFile();
	void throwException(string exception);
};