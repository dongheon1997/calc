#pragma once

#include <iostream>
#include "Stack.h"

using namespace std;

enum { UNARY, DYADIC, OPERAND };

class Calc
{
private:
	string postfix;
	double result;

	static int comingPriority(char oper);
	static bool isDigit(char c);
	void makePostfix(string &infix);
	void calculate();

public:
	void run(string infix);
};

int Calc::comingPriority(char oper)
{
	switch (oper)
	{
	case '(':
		return 0;
	case '+':
	case '-':
		return 1;
	case '*':
	case '/':
	case '%':
		return 2;
	case '^':
		return 3;
	case '~':
		return 4;
	}

	throw -1;
}

inline bool Calc::isDigit(char c)
{
	return (c >= '0' && c <= '9');
}

void Calc::makePostfix(string &infix)
{
	postfix = "";

	const char *str = infix.c_str();
	Stack<char> oper;
	int prev = DYADIC;
	int bracketCount = 0;
	char temp;

	for (;; str += 1)
	{
		if (isDigit(*str))
		{
			if (prev == OPERAND) throw "피연산자가 연속으로 배치될 수 없습니다.";

			do
			{
				postfix.push_back(*(str++));
			} while (isDigit(*str));

			postfix.push_back(' ');
			prev = OPERAND;
		}

		switch (*str)
		{
		case ' ':
			break;

		case '-':
			if (prev != OPERAND)
			{
				oper.push('~');
				prev = UNARY;
				break;
			}

		case '+':
		case '*':
		case '/':
		case '%':
		case '^':
			if (prev != OPERAND) throw "이항연산자 전에 피연산자가 위치해야합니다.";

			while (!oper.isEmpty() && comingPriority(oper.peek()) >= comingPriority(*str))
			{
				postfix.push_back(oper.pop());
				postfix.push_back(' ');
			}

			oper.push(*str);
			prev = DYADIC;
			break;

		case '(':
			if (prev == OPERAND) throw "여는 괄호 전에는 연산자가 위치해야합니다.";

			oper.push(*str);
			bracketCount += 1;
			break;

		case ')':
			if (prev != OPERAND)
				throw "닫는 괄호 전에는 피연산자가 위치해야합니다.";
			else if (bracketCount == 0)
				throw "괄호의 짝이 맞지 않습니다.";

			while (true)
			{
				temp = oper.pop();
				if (temp == '(') break;
				postfix.push_back(temp);
				postfix.push_back(' ');
			}

			bracketCount -= 1;
			break;

		case '\0':
			if (prev != OPERAND)
				throw "식이 비었거나 연산자로 끝났습니다.";
			else if (bracketCount != 0)
				throw "괄호의 짝이 맞지 않습니다.";

			while (!oper.isEmpty())
			{
				postfix.push_back(oper.pop());
				postfix.push_back(' ');
			}
			return;

		default:
			throw "식에 알 수 없는 문자가 있습니다.";
		}
	}
}

void Calc::calculate()
{
	const char *str = postfix.c_str();
	Stack<double> operand;
	double left, right;

	for (; *str; str += 1)
	{
		if (isDigit(*str))
		{
			operand.push(atof(str));

			do
			{
				str += 1;
			} while (isDigit(*str));
		}

		if (*str != ' ')
		{
			right = operand.pop();
			if (*str != '~')
			{
				left = operand.pop();
			}

			switch (*str)
			{
			case '-':
				operand.push(left - right);
				break;
			case '+':
				operand.push(left + right);
				break;
			case '*':
				operand.push(left * right);
				break;
			case '/':
				if (right == 0)
				{
					throw "나누기 연산에서 0으로 나누는 사태가 발생했습니다.";
				}

				operand.push(left / right);
				break;
			case '%':
				if (right == 0)
				{
					throw "나머지 연산에서 0으로 나누는 사태가 발생했습니다.";
				}

				operand.push((int)left % (int)right);
				break;
			case '^':
				operand.push(pow(left, right));
				break;
			case '~':
				operand.push(right * -1);
				break;
			}
		}
	}

	result = operand.pop();
}

void Calc::run(string infix)
{
	try
	{
		makePostfix(infix);

		cout << "후위식은 " << postfix << endl;
	}
	catch (const char *error)
	{
		cout << "[오류] " << error << endl << endl;

		return;
	}

	try
	{
		calculate();

		cout << "결과는 " << result << endl << endl;
	}
	catch (const char *error)
	{
		cout << "[오류] " << error << endl << endl;
	}
}
