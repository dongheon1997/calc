#include <iostream>
#include <math.h>
#include "stack.h"

using namespace std;

//

enum { ���׿�����, ���׿�����, �ǿ����� };

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

	int prevInput = ���׿�����;
	int braceCount = 0;

	while (true)
	{
		if (isDigit(*infix))
		{
			if (prevInput == �ǿ�����)
			{
				throw "�ǿ����ڰ� �������� ��ġ�� �� �����ϴ�.";
			}

			do {
				result[num++] = *(infix++);
			} while (isDigit(*infix));

			result[num++] = ' ';
			prevInput = �ǿ�����;
		}

		switch (*infix)
		{
		case ' ':
			break;
		case '-':
			if (prevInput != �ǿ�����)
			{
				oper.push('~');
				prevInput = ���׿�����;
				break;
			}
		case '+':
		case '*':
		case '/':
		case '%':
		case '^':
			if (prevInput != �ǿ�����)
			{
				throw "���׿����� ���� �����ڰ� ��ġ�� �� �����ϴ�.";
			}

			while (comingPriority(oper.peek()) >= comingPriority(*infix))
			{
				result[num++] = oper.pop();
				result[num++] = ' ';
			}

			oper.push(*infix);
			prevInput = ���׿�����;
			break;
		case '(':
			if (prevInput == �ǿ�����)
			{
				throw "���� ��ȣ ���� �ǿ����ڰ� ��ġ�� �� �����ϴ�.";
			}

			oper.push(*infix);
			braceCount += 1;
			break;
		case ')':
			if (prevInput != �ǿ�����)
			{
				throw "�ݴ� ��ȣ ���� �����ڰ� ��ġ�� �� �����ϴ�.";
			}
			else if (braceCount == 0)
			{
				throw "��ȣ�� ¦�� ���� �ʽ��ϴ�.";
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
			if (prevInput != �ǿ�����)
			{
				throw "���� ����ų� �����ڷ� �������ϴ�.";
			}
			else if (braceCount != 0)
			{
				throw "��ȣ�� ¦�� ���� �ʽ��ϴ�.";
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
			throw "�Ŀ� �� �� ���� ���ڰ� �ֽ��ϴ�.";
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
					throw "������ ���꿡�� 0���� ������ ���°� �߻��߽��ϴ�.";
				}

				num.push(left / right);
				break;
			case '%':
				if (right == 0)
				{
					throw "������ ���꿡�� 0���� ������ ���°� �߻��߽��ϴ�.";
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
		cout << "������ �Է��ϼ���. (����� quit) �� ";
		cin.getline(input, 256);

		if (!strcmp(input, "quit")) break;

		try
		{
			postfix = infixToPostfix(input);
			cout << "�������� " << postfix << endl;
		}
		catch (const char *error)
		{
			cout << "[����] " << error << endl << endl;
			continue;
		}

		try
		{
			cout << calculate(postfix) << endl << endl;
		}
		catch (const char *error)
		{
			cout << "[����] " << error << endl << endl;
			continue;
		}
	}

	return 0;
}
