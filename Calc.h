#pragma once
#include <iostream>
#include <string>
#include "Stack.h"

class Calc
{
private:
	std::string postfix;
	double result;

	enum { UNARY, DYADIC, OPERAND };
	static int comingPriority(char oper);
	static bool isDigit(char c);
	void makePostfix(std::string &infix);
	void calculate();

public:
	void run(std::string infix);
};
