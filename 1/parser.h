#pragma once

#include "function.h"

class Parser
{
public:
	Parser(){}

	Parser(string tokensFileName, string constTableFileName)
	{
		if (table.readTableFromFile("reserved_words.txt"))
		{
			if (readFunctionsFromFile(tokensFileName))
			{
				cout << "Parser did not detect errors." << endl << endl;
			}
			else
			{
				cout << "Errors saved in file \"parser_exceptions.txt\"." << endl << endl;
			}
		}
		else
		{
			cout << "Error. Can't open \"reserved_words.txt\"." << endl << endl;
		}
	}

	~Parser(){}

	void writeOperators(string fileName);
	int result();

private:
	bool mainDescripted = 0;
	ConstTable table;

	bool readFunctionsFromFile(string tokensFileName);
	vector<Function> functions;
};