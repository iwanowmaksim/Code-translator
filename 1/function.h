#pragma once

#include "variable_table.h"
#include <stack>

class Function
{
public:
	Function()
	{
		variables.resize();
	}

	~Function(){}

	void addVariable(string name);
	void addParam(string name);
	void setName(string name);
	string getName();
	int exprResult(vector<Token> expr);
	int result(vector<integer> inputParams, vector<Function> functions);
	bool isVarExist(string name);
	void addOperator(vector<Token> operator_);
	void clearFunction();
	bool isCorrectExpr(vector<Token> expr, vector<Function> functions);
	int getParamsCount();

	vector<vector<Token>> operators;
	VariableTable variables;

private:
	string functionName;
	vector<string> params;
	//VariableTable variables;
	//vector<vector<Token>> operators;
	integer variable, constant;
};