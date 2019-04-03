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

bool isDigit(char c)
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
				strcpy_s(result, "피연산자가 연속으로 배치될 수 없습니다.");
				return result;
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
				strcpy_s(result, "이항연산자 전에 연산자가 위치할 수 없습니다.");
				return result;
			}

			while (comingPriority(oper.checkTop()) >= comingPriority(*infix))
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
				strcpy_s(result, "여는 괄호 전에 피연산자가 위치할 수 없습니다.");
				return result;
			}

			oper.push(*infix);
			braceCount += 1;
			break;
		case ')':
			if (prevInput != 피연산자)
			{
				strcpy_s(result, "닫는 괄호 전에 연산자가 위치할 수 없습니다.");
				return result;
			}
			else if (braceCount == 0)
			{
				strcpy_s(result, "괄호의 짝이 맞지 않습니다.");
				return result;
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
				strcpy_s(result, "식이 비었거나 연산자로 끝났습니다.");
				return result;
			}
			else if (braceCount != 0)
			{
				strcpy_s(result, "괄호의 짝이 맞지 않습니다.");
				return result;
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
			strcpy_s(result, "식에 알 수 없는 문자가 있습니다.");
			return result;
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
					cout << "[오류] 0으로 나누는 사태가 발생했습니다.";
					return DBL_MAX;
				}

				num.push(left / right);
				break;
			case '%':
				if (left - (int)left || right - (int)right)
				{
					cout << "[오류] 정수가 아닌 피연산자 간 나머지 연산이 발생했습니다.";
					return DBL_MAX;
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
	
	cin.getline(input, 256);

	char *postfix = infixToPostfix(input);

	if (isDigit(*postfix))
	{
		cout << "후위식은 " << postfix << endl;

		cout << "결과값은 " << calculate(postfix) << endl;
	}
	else
	{
		cout << postfix << endl;
	}

	return 0;
}
