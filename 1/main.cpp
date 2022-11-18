#include "parser.h"

int main() 
{
	Scaner scaner("input.txt", "output.txt");

	Parser prog("output.txt", "reserved_words.txt");
	prog.writeOperators("Text.txt");
	int a = prog.result();

	system("pause");
	return 0;
}