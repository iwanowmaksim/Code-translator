#include "scaner.h"

void Scaner::correctOperation(string &str, string sign)
{
	// коррректировка двузначных операций

	int index, strIndex = 0;
	vector<int> indexVector;

	while ((index = str.find(sign + " =", strIndex)) != string::npos)
	{
		strIndex = index + 1;
		indexVector.push_back(index);
	}

	for (int i = 0; i < indexVector.size(); i++)
	{
		str.replace(indexVector[i], 3, sign + "=");

		for (int j = i + 1; j < indexVector.size(); j++)
		{
			indexVector[j] -= 1;
		}
	}
}

void Scaner::prepareString(string &str, Token token)
{
	int index;

	//очистка строки от однострочного комментария

	if ((index = str.find("//")) != string::npos)
	{
		str.erase(index);
	}

	//очистка строки от блочных комментариев

	bool detected;
	int openCommentIndex, closeCommentIndex;
	string openComment = "/*", closeComment = "*/";
	
	detected = 1;
	while (detected)
	{
		openCommentIndex = str.find(openComment);
		closeCommentIndex = str.find(closeComment);

		if (openCommentIndex == string::npos && closeCommentIndex == string::npos)
		{
			detected = 0;
		}

		if (activeBlockComment)
		{
			if (closeCommentIndex != string::npos)
			{
				str = str.substr(closeCommentIndex + closeComment.length());
				activeBlockComment = 0;
			}
			else
			{
				str = "";
			}
		}
		else
		{
			if (openCommentIndex == string::npos && closeCommentIndex != string::npos)
			{
				token.name = closeComment;
				token.type = -1;
				wrongTokens.push_back(token);
				detected = 0;
			}

			if (openCommentIndex != string::npos && closeCommentIndex == string::npos)
			{
				str = str.erase(openCommentIndex);
				activeBlockComment = 1;
				detected = 0;
			}

			if (openCommentIndex != string::npos && closeCommentIndex != string::npos)
			{
				if (closeCommentIndex < openCommentIndex)
				{
					token.name = closeComment;
					token.type = -1;
					wrongTokens.push_back(token);
					detected = 0;
				}
				else
				{
					if (closeCommentIndex == openCommentIndex + 1)
					{
						token.name = "/*/";
						token.type = -1;
						wrongTokens.push_back(token);
						detected = 0;
					}
					else
					{
						str.replace(openCommentIndex, closeCommentIndex - openCommentIndex + closeComment.length(), "");
						activeBlockComment = 0;
					}
				}
			}
		}
	}

	//отделение разделителей и знаков операций

	int strIndex;
	vector<int> indexVector;

	for (auto el : separators)
	{
		strIndex = 0;
		indexVector.clear();

		while ((index = str.find(el, strIndex)) != string::npos)
		{
			strIndex = index + 1;
			indexVector.push_back(index);
		}

		for (int i = 0; i < indexVector.size(); i++)
		{
			str.replace(indexVector[i], el.length(), " " + el + " ");

			for (int j = i + 1; j < indexVector.size(); j++)
			{
				indexVector[j] += 2;
			}
		}
	}

	for (auto el : operations)
	{
		strIndex = 0;
		indexVector.clear();

		while ((index = str.find(el, strIndex)) != string::npos)
		{
			strIndex = index + 1;
			indexVector.push_back(index);
		}

		for (int i = 0; i < indexVector.size(); i++)
		{
			str.replace(indexVector[i], el.length(), " " + el + " ");

			for (int j = i + 1; j < indexVector.size(); j++)
			{
				indexVector[j] += 2;
			}
		}
	}

	//очистака строки от лишних пробелов и табуляций

	if (!str.empty())
	{
		while ((str[0] == ' ' || str[0] == '\t') && str.size() > 0)
		{
			str = str.substr(1);
		}
	}

	if (!str.empty())
	{
		while ((str[str.size() - 1] == ' ' || str[str.size() - 1] == '\t') && str.size() > 0)
		{
			str = str.erase(str.size() - 1);
		}
	}

	while ((index = str.find("  ", strIndex)) != string::npos)
	{
		str.replace(index, 2, " ");
	}

	correctOperation(str, "!");
	correctOperation(str, "=");
	correctOperation(str, "+");
	correctOperation(str, "-");
}

void Scaner::setTokenType(string subStr, Token token)
{
	bool correctToken = 0;

	token.name = subStr;

	//определение типа токена. Если не найдется подходящий, ошибка.

	//операция
	if (!correctToken)
	{
		for (auto operation : operations)
		{
			if (token.name == operation)
			{
				correctToken = 1;
				token.type = 2;
				break;
			}
		}
	}

	//разделитель
	if (!correctToken)
	{
		for (auto separator : separators)
		{
			if (token.name == separator)
			{
				correctToken = 1;
				token.type = 3;
				break;
			}
		}
	}

	//ключевое слово
	if (!correctToken)
	{
		for (auto reserved_word : reserved_words)
		{
			if (token.name == reserved_word)
			{
				correctToken = 1;
				token.type = 4;
				break;
			}
		}
	}

	//идентификатор
	if (!correctToken)
	{
		if (token.name[0] == '_' || ('a' <= token.name[0] && token.name[0] <= 'z')
			|| ('A' <= token.name[0] && token.name[0] <= 'Z'))
		{
			correctToken = 1;
			token.type = 0;

			for (int i = 1; i < token.name.size(); i++)
			{
				if (!(token.name[i] == '_' || ('a' <= token.name[i] && token.name[i] <= 'z')
					|| ('A' <= token.name[i] && token.name[i] <= 'Z') || isdigit(token.name[i])))
				{
					correctToken = 0;
					break;
				}
			}

			if (token.name[0] == '_' && token.name.size() < 2)
			{
				correctToken = 0;
			}
		}
	}

	//константа
	if (!correctToken)
	{
		correctToken = 1;
		token.type = 1;

		for (int i = 0; i < token.name.size(); i++)
		{
			if (!isdigit(token.name[i]))
			{
				correctToken = 0;
				break;
			}
		}
	}

	if (!correctToken)
	{
		token.type = -1;
		wrongTokens.push_back(token);
	}
	else
	{
		correctTokens.push_back(token);
	}
}

void Scaner::lineProcessing(string str, int laneNumber)
{
	string word;

	Token token;
	token.laneNumber = laneNumber;

	prepareString(str, token);

	if (!wrongTokens.size())
	{
		if (!str.empty())
		{
			for (size_t p = 0, q = 0; p != string::npos; p = q)
			{
				//разбение строки на слова

				word = str.substr(p + (p != 0), (q = str.find(' ', p + 1)) - p - (p != 0));

				setTokenType(word, token);
			}
		}
	}
}

bool Scaner::readWordsFromFile(string fileName, vector<string> &vec)
{
	ifstream file(fileName.c_str());

	if (file.is_open())
	{
		string str;

		while (!file.eof())
		{
			file >> str;
			vec.push_back(str);
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

bool Scaner::readTokensFromFile(string fileName)
{
	ifstream file(fileName.c_str());

	if (file.is_open())
	{
		int laneNumber = 1;
		string str;

		while (!file.eof())
		{
			getline(file, str);
			lineProcessing(str, laneNumber);
			laneNumber++;
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

bool Scaner::writeTokensToFile(string fileName)
{
	ofstream file(fileName.c_str());
	ofstream fileEx("scanner_exceptions.txt");

	if (wrongTokens.size() > 0)
	{
		for (auto el : wrongTokens)
		{
			fileEx << "Lexical error in line " << el.laneNumber << ": " << el.name << "." << endl;
			cout << "Lexical error in line " << el.laneNumber << ": " << el.name << "." << endl;
		}

		fileEx.close();
		file.close();
		return false;
	}
	else
	{
		for (auto el : correctTokens)
		{
			file << el.type << " " << el.name << " " << el.laneNumber << endl;
		}

		fileEx.close();
		file.close();
		return true;
	}
}