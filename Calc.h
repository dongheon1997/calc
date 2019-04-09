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
			if (prev == OPERAND) throw "�ǿ����ڰ� �������� ��ġ�� �� �����ϴ�.";

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
			if (prev != OPERAND) throw "���׿����� ���� �ǿ����ڰ� ��ġ�ؾ��մϴ�.";

			while (!oper.isEmpty() && comingPriority(oper.peek()) >= comingPriority(*str))
			{
				postfix.push_back(oper.pop());
				postfix.push_back(' ');
			}

			oper.push(*str);
			prev = DYADIC;
			break;

		case '(':
			if (prev == OPERAND) throw "���� ��ȣ ������ �����ڰ� ��ġ�ؾ��մϴ�.";

			oper.push(*str);
			bracketCount += 1;
			break;

		case ')':
			if (prev != OPERAND)
				throw "�ݴ� ��ȣ ������ �ǿ����ڰ� ��ġ�ؾ��մϴ�.";
			else if (bracketCount == 0)
				throw "��ȣ�� ¦�� ���� �ʽ��ϴ�.";

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
				throw "���� ����ų� �����ڷ� �������ϴ�.";
			else if (bracketCount != 0)
				throw "��ȣ�� ¦�� ���� �ʽ��ϴ�.";

			while (!oper.isEmpty())
			{
				postfix.push_back(oper.pop());
				postfix.push_back(' ');
			}
			return;

		default:
			throw "�Ŀ� �� �� ���� ���ڰ� �ֽ��ϴ�.";
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
					throw "������ ���꿡�� 0���� ������ ���°� �߻��߽��ϴ�.";
				}

				operand.push(left / right);
				break;
			case '%':
				if (right == 0)
				{
					throw "������ ���꿡�� 0���� ������ ���°� �߻��߽��ϴ�.";
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

		cout << "�������� " << postfix << endl;
	}
	catch (const char *error)
	{
		cout << "[����] " << error << endl << endl;

		return;
	}

	try
	{
		calculate();

		cout << "����� " << result << endl << endl;
	}
	catch (const char *error)
	{
		cout << "[����] " << error << endl << endl;
	}
}
