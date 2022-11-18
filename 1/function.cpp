#include "function.h"

void Function::addVariable(string name)
{
	variables.addItem(name);
}

void Function::addParam(string name)
{
	params.push_back(name);
}

void Function::setName(string name)
{
	functionName = name;
}

string Function::getName()
{
	return functionName;
}

int Function::exprResult(vector<Token> expr)
{
	int returnedValue = 0;

	stack<Token> opstack;
	vector<Token> outVec;

	for (int i = 0; i < expr.size(); i++)
	{
		if (expr[i].type == 0 || expr[i].type == 1)
		{
			outVec.push_back(expr[i]);
		}
		else
		{
			if (expr[i].name == "(")
			{
				opstack.push(expr[i]);
			}
			else
			{
				if (expr[i].name == ")")
				{
					Token token = opstack.top();
					opstack.pop();

					while (token.name != "(")
					{
						outVec.push_back(token);
						token = opstack.top();
						opstack.pop();
						outVec.push_back(token);
					}	
				}
				else
				{
					if (expr[i].name == "+" || expr[i].name == "-")
					{
						Token token;
						bool b = 1;
						while (opstack.size() > 0 && b)
						{
							token = opstack.top();
							if (token.name == "+" || token.name == "-")
							{
								opstack.pop();
								outVec.push_back(token);
							}
							else
							{
								b = 0;
							}
						}
						opstack.push(expr[i]);
					}
					else
					{
						if (expr[i].name == "!=" || expr[i].name == "==" || expr[i].name == ">" || expr[i].name == "<")
						{
							Token token;
							bool b = 1;
							while (opstack.size() > 0 && b)
							{
								token = opstack.top();
								if (token.name == "+" || token.name == "-" || token.name == "!=" || token.name == "==" || token.name == ">" || token.name == "<")
								{
									opstack.pop();
									outVec.push_back(token);
								}
								else
								{
									b = 0;
								}
							}
							opstack.push(expr[i]);
						}
						else
						{
							if (expr[i].name == "=" || expr[i].name == "+=" || expr[i].name == "-=")
							{
								Token token;
								bool b = 1;
								while (opstack.size() > 0 && b)
								{
									token = opstack.top();
									if (expr[i].name == "=" || expr[i].name == "+=" || expr[i].name == "-=" || token.name == "+" || token.name == "-" || token.name == "!=" || token.name == "==" || token.name == ">" || token.name == "<")
									{
										opstack.pop();
										outVec.push_back(token);
									}
									else
									{
										b = 0;
									}
								}
								opstack.push(expr[i]);
							}
						}
					}
				}
			}
		}
			
		Token token;
		while (opstack.size() > 0)
		{
			token = opstack.top();
			opstack.pop();
			outVec.push_back(token);
		}
	}

	ofstream file("polish.txt", ios::app);

	for (auto el : outVec)
	{
		file << el.name << " ";
	}
	file << endl;

	file.close();

	return returnedValue;
}

int Function::result(vector<integer> inputParams, vector<Function> functions)
{
	ofstream fileEx("logic_exceptions.txt");

	bool someEx = 0;
	int returnedValue = 0;

	if (inputParams.size() != params.size())
	{
		fileEx << "Syntax error in function " << ": " << functionName << ". Wrong count of parameters." << endl;
		cout << "Syntax error in function " << ": " << functionName << ". Wrong count of parameters." << endl;
		someEx = 1;
	}
	else
	{
		bool returned = 0;

		for (int i = 0; i < inputParams.size(); i++)
		{
			variables.setItemValue(params[i], inputParams[i].value);
		}

		for (auto operator_ : operators)
		{
			if (operator_.size() > 0)
			{
				if (operator_[0].name == "int")
				{
					if (operator_.size() > 1)
					{
						if (operator_[1].type == 0)
						{
							bool brokenName = 0;

							for (auto el : functions)
							{
								if (el.getName() == operator_[1].name)
								{
									brokenName = 1;
									break;
								}
							}

							if (brokenName)
							{
								fileEx << "Syntax error in function " << ": " << functionName << ". Using name of function in line " << operator_[1].laneNumber << ": " << operator_[1].name << "." << endl;
								cout << "Syntax error in function " << ": " << functionName << ". Using name of function in line " << operator_[1].laneNumber << ": " << operator_[1].name << "." << endl;
								someEx = 1;
							}
							else
							{
								if (variables.checkItem(operator_[1].name))
								{
									fileEx << "Syntax error in function " << ": " << functionName << ". Redefinition of variable in line " << operator_[1].laneNumber << ": " << operator_[1].name << "." << endl;
									cout << "Syntax error in function " << ": " << functionName << ". Redefinition of variable in line " << operator_[1].laneNumber << ": " << operator_[1].name << "." << endl;
									someEx = 1;
								}
								else
								{
									variables.addItem(operator_[1].name);

									if (operator_.size() > 2)
									{
										if (operator_[2].name == "=")
										{
											if (operator_.size() > 3)
											{
												vector<Token> expr;
												for (int i = 3; i < operator_.size(); i++)
												{
													expr.push_back(operator_[i]);
												}

												if (isCorrectExpr(expr, functions))
												{
													variables.setItemValue(operator_[1].name, exprResult(expr));
												}
											}
											else
											{
												fileEx << "Syntax error in function " << ": " << functionName << ". Expected expression in line " << operator_[2].laneNumber << "." << endl;
												cout << "Syntax error in function " << ": " << functionName << ". Expected expression in line " << operator_[2].laneNumber << "." << endl;
												someEx = 1;
											}
										}
										else
										{
											fileEx << "Syntax error in function " << ": " << functionName << ". Expected \"=\" in line " << operator_[2].laneNumber << ": " << operator_[2].name << "." << endl;
											cout << "Syntax error in function " << ": " << functionName << ". Expected \"=\" in line " << operator_[2].laneNumber << ": " << operator_[2].name << "." << endl;
											someEx = 1;
										}	
									}
								}
							}
						}
						else
						{
							fileEx << "Syntax error in function " << ": " << functionName << ". Expected name of variable in line " << operator_[0].laneNumber << ": " << operator_[1].name << "." << endl;
							cout << "Syntax error in function " << ": " << functionName << ". Expected name of variable in line " << operator_[0].laneNumber << ": " << operator_[1].name << "." << endl;
							someEx = 1;
						}
					}
					else
					{
						fileEx << "Syntax error in function " << ": " << functionName << ". Expected name of variable in line " << operator_[0].laneNumber << "." << endl;
						cout << "Syntax error in function " << ": " << functionName << ". Expected name of variable in line " << operator_[0].laneNumber << "." << endl;
						someEx = 1;
					}
				}
				else
				{
					if (operator_[0].type == 0)
					{
						if (variables.checkItem(operator_[0].name))
						{
							if (operator_.size() > 1)
							{
								if (operator_[1].name == "=" || operator_[1].name == "+=" || operator_[1].name == "-=")
								{
									if (operator_.size() > 2)
									{
										vector<Token> expr;
										for (int i = 2; i < operator_.size(); i++)
										{
											expr.push_back(operator_[i]);
										}

										if (isCorrectExpr(expr, functions))
										{
											if (operator_[1].name == "=")
											{
												variables.setItemValue(operator_[1].name, exprResult(expr));
											}
											else
											{
												if (!variables.isInitItem(operator_[0].name))
												{
													fileEx << "Syntax error in function " << ": " << functionName << ". Using of udefined variable in line " << operator_[0].laneNumber << ": " << operator_[0].name << "." << endl;
													cout << "Syntax error in function " << ": " << functionName << ". Using of udefined variable in line " << operator_[0].laneNumber << ": " << operator_[0].name << "." << endl;
													someEx = 1;
												}
												else
												{
													if (operator_[1].name == "+=")
													{
														variables.setItemValue(operator_[1].name, variables.getValueByName(operator_[1].name) + exprResult(expr));
													}
													else
													{
														variables.setItemValue(operator_[1].name, variables.getValueByName(operator_[1].name) - exprResult(expr));
													}
												}
											}
										}
									}
									else
									{
										fileEx << "Syntax error in function " << ": " << functionName << ". Expected expression in line " << operator_[1].laneNumber << "." << endl;
										cout << "Syntax error in function " << ": " << functionName << ". Expected expression in line " << operator_[1].laneNumber << "." << endl;
										someEx = 1;
									}
								}
								else
								{
									fileEx << "Syntax error in function " << ": " << functionName << ". Expected \"=\" or \"+=\" or \"-=\" in line " << operator_[1].laneNumber << ": " << operator_[1].name << "." << endl;
									cout << "Syntax error in function " << ": " << functionName << ". Expected \"=\" or \"+=\" or \"-=\" in line " << operator_[1].laneNumber << ": " << operator_[1].name << "." << endl;
									someEx = 1;
								}
							}
						}
						else
						{
							fileEx << "Syntax error in function " << ": " << functionName << ". Using of unexpected variable in line " << operator_[0].laneNumber << ": " << operator_[0].name << "." << endl;
							cout << "Syntax error in function " << ": " << functionName << ". Using of unexpected variable in line " << operator_[0].laneNumber << ": " << operator_[0].name << "." << endl;
							someEx = 1;
						}
					}
					else
					{
						if (operator_[0].name == "return")
						{
							if (operator_.size() > 1)
							{
								vector<Token> expr;
								for (int i = 1; i < operator_.size(); i++)
								{
									expr.push_back(operator_[i]);
									if (isCorrectExpr(expr, functions))
									{
										if (!returned)
										{
											returnedValue = exprResult(expr);
											returned = 1;
										}
									}
									else
									{
										fileEx << "Syntax error in function " << ": " << functionName << ". Wrong expression in line " << operator_[0].laneNumber << "." << endl;
										cout << "Syntax error in function " << ": " << functionName << ". Wrong expression in line " << operator_[0].laneNumber << "." << endl;
										someEx = 1;
									}
								}
							}
							else
							{
								fileEx << "Syntax error in function " << ": " << functionName << ". Expected expression in line " << operator_[0].laneNumber << "." << endl;
								cout << "Syntax error in function " << ": " << functionName << ". Expected expression in line " << operator_[0].laneNumber << "." << endl;
								someEx = 1;
							}
						}
						else
						{
							fileEx << "Syntax error in function " << ": " << functionName << ". Unexpected expression in line " << operator_[0].laneNumber << ": " << operator_[0].name << "." << endl;
							cout << "Syntax error in function " << ": " << functionName << ". Unexpected expression in line " << operator_[0].laneNumber << ": " << operator_[0].name << "." << endl;;
							someEx = 1;
						}
					}
				}
			}
		}
	}
	
	if (!someEx)
	{
		cout << "Not detect logic errors." << endl << endl;
	}
	else
	{
		cout << "Errors saved in file \"logic_exceptions.txt\"." << endl << endl;
	}

	fileEx.close();
	return returnedValue;
}

bool Function::isVarExist(string name)
{
	return variables.checkItem(name);
}

void Function::addOperator(vector<Token> operator_)
{
	operators.push_back(operator_);
}

void Function::clearFunction()
{
	operators.clear();
	params.clear();
	variables.clear();
	variables.resize();
}

bool Function::isCorrectExpr(vector<Token> expr, vector<Function> functions)
{
	for (auto el : expr)
	{
		if (!variables.checkItem(el.name) && el.name != "+" && el.name != "-" && el.name != ")" && el.name != "("
			&& el.name != ">" && el.name != "<" && el.name != "," && el.name != "!=" && el.name != "==")
		{
			bool correct = 0;

			for (auto f : functions)
			{
				if (f.getName() == el.name)
				{
					correct = 1;
					break;
				}
			}

			if (!correct)
			{
				correct = 1;
				for (auto ch : el.name)
				{
					if (!isdigit(ch))
					{
						correct = 0;
						break;
					}
				}
			}

			if (!correct)
			{
				return 0;
			}
		}
	}

	for (int i = 0; i < expr.size() - 1; i++)
	{
		if ((expr[i].type == 0 || expr[i].type == 1) && (expr[i + 1].type == 0 || expr[i + 1].type == 1))
		{
			cout << "2 identificators/constants in a row" << endl;
			return 0;
		}
		else
		{
			if ((expr[i].type == 2 || expr[i].type == 3) && (expr[i + 1].type == 2 || expr[i + 1].type == 3))
			{
				cout << "2 operators/separators in a row" << endl;
				return 0;
			}
		}
	}

	int openBracket = 0, closeBracket = 0;
	for (auto el : expr)
	{
		if (el.name == "(")
		{
			openBracket++;
		}
		else
		{
			if (el.name == ")")
			{
				closeBracket++;
			}
		}
	}
	if (openBracket != closeBracket)
	{
		cout << "Unequal number of brackets" << endl;
		return 0;
	}

	for (int i = 0; i < expr.size(); i++)
	{
		for (auto f : functions)
		{
			if (f.getName() == expr[i].name)
			{
				if (i + 2 > expr.size())
				{
					cout << "Expected function \"" << f.getName() << "\" parameters." << endl;
					return 0;
				}
				else
				{
					int countOfParams = 0;
					int j = i + 2;

					while (expr[j].name != ")" && j < expr.size())
					{
						if (expr[j].type == 0)
						{
							countOfParams++;
						}

						j++;
					}

					if (expr[j].name != ")")
					{
						cout << "Expected function \")\"." << endl;
						return 0;
					}

					if (countOfParams != f.getParamsCount())
					{
						cout << "Wrong count of parameters in function \"" << f.getName() << "\". Expected: " << f.getParamsCount() << "." << endl;
						return 0;
					}
				}

				break;
			}
		}
	}

	return 1;
}

int Function::getParamsCount()
{
	return params.size();
}