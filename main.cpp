#include <iostream>
#include <math.h>
#include "stack.h"

using namespace std;

//

enum { 이항연산자, 단항연산자, 피연산자 };

int comingPriority(char oper)
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
	default:
		return -1;
	}
}

inline bool isDigit(char c)
{
	return (c >= '0' && c <= '9');
}

char *infixToPostfix(char *infix)
{
	static char result[256];
	int num = 0;
	stack<char> oper;
	char temp;

	int prevInput = 이항연산자;
	int braceCount = 0;

	while (true)
	{
		if (isDigit(*infix))
		{
			if (prevInput == 피연산자)
			{
				throw "피연산자가 연속으로 배치될 수 없습니다.";
			}

			do {
				result[num++] = *(infix++);
			} while (isDigit(*infix));

			result[num++] = ' ';
			prevInput = 피연산자;
		}

		switch (*infix)
		{
		case ' ':
			break;
		case '-':
			if (prevInput != 피연산자)
			{
				oper.push('~');
				prevInput = 단항연산자;
				break;
			}
		case '+':
		case '*':
		case '/':
		case '%':
		case '^':
			if (prevInput != 피연산자)
			{
				throw "이항연산자 전에 연산자가 위치할 수 없습니다.";
			}

			while (comingPriority(oper.peek()) >= comingPriority(*infix))
			{
				result[num++] = oper.pop();
				result[num++] = ' ';
			}

			oper.push(*infix);
			prevInput = 이항연산자;
			break;
		case '(':
			if (prevInput == 피연산자)
			{
				throw "여는 괄호 전에 피연산자가 위치할 수 없습니다.";
			}

			oper.push(*infix);
			braceCount += 1;
			break;
		case ')':
			if (prevInput != 피연산자)
			{
				throw "닫는 괄호 전에 연산자가 위치할 수 없습니다.";
			}
			else if (braceCount == 0)
			{
				throw "괄호의 짝이 맞지 않습니다.";
			}

			while (true)
			{
				temp = oper.pop();
				if (temp == '(') break;
				result[num++] = temp;
				result[num++] = ' ';
			}
			braceCount -= 1;
			break;
		case '\0':
			if (prevInput != 피연산자)
			{
				throw "식이 비었거나 연산자로 끝났습니다.";
			}
			else if (braceCount != 0)
			{
				throw "괄호의 짝이 맞지 않습니다.";
			}

			while (true)
			{
				temp = oper.pop();
				result[num++] = temp;
				result[num++] = ' ';
				if (temp == NULL) return result;
			}
			break;
		default:
			throw "식에 알 수 없는 문자가 있습니다.";
		}

		infix += 1;
	}
}

double calculate(char *postfix)
{
	stack<double> num;
	double left, right;

	while (*postfix)
	{
		if (isDigit(*postfix))
		{
			num.push(atof(postfix));

			do {
				postfix += 1;
			} while (isDigit(*postfix));
		}
		
		if (*postfix != ' ')
		{
			right = num.pop();
			if (*postfix != '~')
			{
				left = num.pop();
			}

			switch (*postfix)
			{
			case '-':
				num.push(left - right);
				break;
			case '+':
				num.push(left + right);
				break;
			case '*':
				num.push(left * right);
				break;
			case '/':
				if (right == 0)
				{
					throw "나누기 연산에서 0으로 나누는 사태가 발생했습니다.";
				}

				num.push(left / right);
				break;
			case '%':
				if (right == 0)
				{
					throw "나머지 연산에서 0으로 나누는 사태가 발생했습니다.";
				}

				num.push((int)left % (int)right);
				break;
			case '^':
				num.push(pow(left, right));
				break;
			case '~':
				num.push(right * -1);
				break;
			}
		}

		postfix += 1;
	}

	return num.pop();
}

int main()
{
	char input[256];
	char *postfix;
	
	while (true)
	{
		cout << "계산식을 입력하세요. (종료는 quit) ☞ ";
		cin.getline(input, 256);

		if (!strcmp(input, "quit")) break;

		try
		{
			postfix = infixToPostfix(input);
			cout << "후위식은 " << postfix << endl;
		}
		catch (const char *error)
		{
			cout << "[오류] " << error << endl << endl;
			continue;
		}

		try
		{
			cout << calculate(postfix) << endl << endl;
		}
		catch (const char *error)
		{
			cout << "[오류] " << error << endl << endl;
			continue;
		}
	}

	return 0;
}
