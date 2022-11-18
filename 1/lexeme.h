#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

struct integer 
{
	string name;
	int value;
	bool isInit;

	integer() 
	{
		value = 0;
		isInit = 0;
	}

	integer(string ind_name) 
	{
		name = ind_name;
		value = 0;
		isInit = 0;
	}

	bool operator < (const integer var)
	{
		return value < var.value;
	}

	bool operator > (const integer var)
	{
		return value > var.value;
	}

	bool operator == (const integer var)
	{
		return value == var.value;
	}

	bool operator != (const integer var)
	{
		return value != var.value;
	}

	integer operator + (const integer var)
	{
		value = value + var.value;
		return *this;
	}

	integer operator - (const integer var)
	{
		value = value - var.value;
		return *this;
	}

	integer operator = (const integer var)
	{
		isInit = 1;
		value = var.value;
		return *this;
	}

	integer operator += (const integer var)
	{
		value += var.value;
		return *this;
	}

	integer operator -= (const integer var)
	{
		value -= var.value;
		return *this;
	}
};