#include "parser.h"

bool Parser::readFunctionsFromFile(string tokensFileName)
{
	ifstream file(tokensFileName.c_str());

	if (file.is_open())
	{
		ofstream fileEx("parser_exceptions.txt");
		bool isFunctionExist = 0, someErrors = 0;
		Function function;
		Token token;
		vector<Token> operator_;

		while (!file.eof())
		{
			if (mainDescripted)
			{
				if (token.name == "}")
				{
					file >> token.type >> token.name >> token.laneNumber;
					if (!file.eof())
					{
						fileEx << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Description after main function." << endl;
						cout << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Description after main function." << endl;
						someErrors = 1;
					}
				}
				else
				{
					file >> token.type >> token.name >> token.laneNumber;
					fileEx << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Description after main function." << endl;
					cout << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Description after main function." << endl;
					someErrors = 1;
				}
				
			}
			else
			{
				file >> token.type >> token.name >> token.laneNumber;

				if (token.name != "int")
				{
					fileEx << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Expected name of type." << endl;
					cout << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Expected name of type." << endl;
					someErrors = 1;
				}
				else
				{
					if (!file.eof())
					{
						file >> token.type >> token.name >> token.laneNumber;

						if (table.isInTheTable(token.name))
						{
							fileEx << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Using of reserved word." << endl;
							cout << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Using of reserved word." << endl;
							someErrors = 1;
						}
						else
						{
							isFunctionExist = 0;
							for (auto el : functions)
							{
								if (el.getName() == token.name)
								{
									isFunctionExist = 1;
								}
							}

							if (isFunctionExist)
							{
								fileEx << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Using of used name of function." << endl;
								cout << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Using of used name of function." << endl;
								someErrors = 1;
							}
							else
							{
								function.setName(token.name);
								function.clearFunction();

								if (token.name == "main")
								{
									mainDescripted = 1;
								}

								if (!file.eof())
								{
									file >> token.type >> token.name >> token.laneNumber;

									if (token.name != "(")
									{
										fileEx << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Expected \"(\"." << endl;
										cout << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Expected \"(\"." << endl;
										someErrors = 1;
									}
									else
									{
										while (token.name != ")")
										{
											if (!file.eof())
											{
												file >> token.type >> token.name >> token.laneNumber;

												if (token.name == ")")
												{
													continue;
												}
												else
												{
													if (token.name == "main")
													{
														fileEx << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Expected \")\"." << endl;
														cout << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Expected \")\"." << endl;
														someErrors = 1;
													}
													else
													{
														if (token.name != "int")
														{
															fileEx << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Expected name of type." << endl;
															cout << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Expected name of type." << endl;
															someErrors = 1;
														}
														else
														{
															if (!file.eof())
															{
																file >> token.type >> token.name >> token.laneNumber;

																if (token.type != 0)
																{
																	fileEx << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Expected name of variable." << endl;
																	cout << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Expected name of variable." << endl;
																	someErrors = 1;
																}
																else
																{
																	if (table.isInTheTable(token.name))
																	{
																		fileEx << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Using of reserved word." << endl;
																		cout << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Using of reserved word." << endl;
																		someErrors = 1;
																	}
																	else
																	{
																		isFunctionExist = 0;
																		for (auto el : functions)
																		{
																			if (el.getName() == token.name)
																			{
																				isFunctionExist = 1;
																			}
																		}

																		if (isFunctionExist)
																		{
																			fileEx << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Using of name of function." << endl;
																			cout << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Using of name of function." << endl;
																			someErrors = 1;
																		}
																		else
																		{
																			if (function.isVarExist(token.name))
																			{
																				fileEx << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Using of used name of variable." << endl;
																				cout << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Using of used name of variable." << endl;
																				someErrors = 1;
																			}
																			else
																			{
																				function.addVariable(token.name);
																				function.addParam(token.name);

																				if (!file.eof())
																				{
																					file >> token.type >> token.name >> token.laneNumber;

																					if (token.name != "," && token.name != ")")
																					{
																						fileEx << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Expected \",\" or \")\"." << endl;
																						cout << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Expected \",\" or \")\"." << endl;
																						someErrors = 1;
																					}
																				}
																				else
																				{
																					fileEx << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Expected \",\" or \")\"." << endl;
																					cout << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Expected \",\" or \")\"." << endl;
																					someErrors = 1;
																				}
																			}
																		}
																	}
																}
															}
															else
															{
																fileEx << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Expected name of variable." << endl;
																cout << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Expected name of variable." << endl;
																someErrors = 1;
															}
														}
													}
												}
											}
											else
											{
												fileEx << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Expected \")\"." << endl;
												cout << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Expected \")\"." << endl;
												someErrors = 1;
											}
										}

										if (!file.eof())
										{
											file >> token.type >> token.name >> token.laneNumber;

											if (token.name != "{")
											{
												fileEx << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Expected \"{\"." << endl;
												cout << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Expected \"{\"." << endl;
												someErrors = 1;
											}
											else
											{
												while (token.name != "}")
												{
													if (!file.eof())
													{
														file >> token.type >> token.name >> token.laneNumber;

														if (token.name == "}")
														{
															continue;
														}
														else
														{
															operator_.clear();

															while (token.name != ";")
															{
																if (!file.eof())
																{
																	if (token.name == ";")
																	{
																		continue;
																	}
																	else
																	{
																		operator_.push_back(token);
																	}

																	file >> token.type >> token.name >> token.laneNumber;
																}
																else
																{
																	fileEx << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Expected \";\"." << endl;
																	cout << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Expected \";\"." << endl;
																	someErrors = 1;
																}
															}

															function.addOperator(operator_);
														}
													}
													else
													{
														fileEx << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Expected \"}\"." << endl;
														cout << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Expected \"}\"." << endl;
														someErrors = 1;
													}
												}

												functions.push_back(function);
											}
										}
									}
								}
								else
								{
									fileEx << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Expected \"(\"." << endl;
									cout << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Expected \"(\"." << endl;
									someErrors = 1;
								}
							}
						}
					}
					else
					{
						fileEx << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Expected name of function." << endl;
						cout << "Syntax error in line " << token.laneNumber << ": " << token.name << ". Expected name of function." << endl;
						someErrors = 1;
					}
				}
			}			
		}

		file.close();
		fileEx.close();

		return !someErrors;
	}
	else
	{
		file.close();
		return 0;
	}
}

int Parser::result()
{
	for (auto el : functions)
	{
		if (el.getName() == "main")
		{
			vector<integer> v;
			return el.result(v, functions);
		}
	}

	cout << "Error. \"main\" function is not descripted." << endl;
	return 0;
}

void Parser::writeOperators(string fileName)
{
	ofstream file(fileName);

	for (auto el : functions)
	{
		file <<  "Function name: " << el.getName() << endl;
		file << "Operators list:" << endl;
		for (auto el_ : el.operators)
		{
			if (el_.size())
			{
				for (auto el__ : el_)
				{
					file << el__.name << " ";
				}

				el.exprResult(el_);

				file << endl;
			}
		}

		file << endl;
	}

	file.close();
}