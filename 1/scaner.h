#pragma once

#include "lexeme.h"

struct Token
{
	int type;
	/*
   -1 - error
	0 - id
	1 - constant
	2 - operation
	3 - separator
	4 - reserved_word
	*/
	int laneNumber;
	string name;
};

class Scaner
{
public:
	Scaner(){}

	Scaner(string inputFileName, string outputFileName)
	{
		bool correctOpen = 1;

		if (!readWordsFromFile("reserved_words.txt", reserved_words))
		{
			cout << "Error. Can't open \"reserved_words.txt\"." << endl << endl;
			correctOpen = 0;
		}

		if (!readWordsFromFile("operations.txt", operations))
		{
			cout << "Error. Can't open \"operations.txt\"." << endl << endl;
			correctOpen = 0;
		}

		if (!readWordsFromFile("separators.txt", separators))
		{
			cout << "Error. Can't open \"separators.txt\"." << endl << endl;
			correctOpen = 0;
		}

		if (correctOpen && !readTokensFromFile(inputFileName))
		{
			cout << "Error. Can't open \"" << inputFileName << "\"." << endl << endl;
			correctOpen = 0;
		}

		if (correctOpen)
		{
			if (writeTokensToFile(outputFileName))
			{
				cout << "Scanner did not detect errors." << endl << endl;
			}
			else
			{
				cout << "Errors saved in file \"scanner_exceptions.txt\"." << endl << endl;
			}
		}	
	}

	~Scaner(){}

private:
	bool activeBlockComment = 0;

	void correctOperation(string &str, string sign);
	void prepareString(string &str, Token token);
	void setTokenType(string str, Token token);
	void lineProcessing(string str, int laneNumber);

	bool readWordsFromFile(string fileName, vector<string> &vec);
	bool readTokensFromFile(string fileName);
	bool writeTokensToFile(string fileName);

	vector<string> separators;
	vector<string> operations;
	vector<string> reserved_words;
	vector<Token> correctTokens;
	vector<Token> wrongTokens;
};