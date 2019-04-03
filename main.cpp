#include <iostream>
#include <math.h>

using namespace std;

template <typename T>
class stack
{
private:
	int top;
	int capacity;
	T *arr;

public:
	stack();
	~stack();
	void resize(bool b);
	void push(T value);
	T pop();
	T check() const;
};

template <typename T>
stack<T>::stack()
{
	top = 0;
	capacity = 8;
	arr = new T[capacity];
}

template <typename T>
stack<T>::~stack()
{
	delete[] arr;
}

template <typename T>
void stack<T>::resize(bool b)
{
	if (b) capacity *= 2;
	else capacity /= 2;

	T *temp = new T[capacity];

	for (int i = 0; i < top; i += 1)
	{
		temp[i] = arr[i];
	}

	delete[] arr;

	arr = temp;
}

template <typename T>
void stack<T>::push(T value)
{
	if (top == capacity)
	{
		resize(true);
	}

	arr[top++] = value;
}

template <typename T>
T stack<T>::pop()
{
	if (top == 0) return NULL;

	if (--top == capacity / 4 && capacity - 8)
	{
		resize(false);
	}

	return arr[top];
}

template <typename T>
T stack<T>::check() const
{
	if (top == 0) return NULL;

	return arr[top - 1];
}

//

int �켱����(char c)
{
	switch (c)
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

	return -1;
}

enum {���׿�����, ���׿�����, ����};

char *calculate(char *str) // ���������� �����ϴ� �Լ�
{
	static char result[256];
	int num = 0;

	stack<char> oper;

	int prev = ���׿�����;
	int ��ȣī���� = 0;

	while (true) // �� ���� ���������� �ߵǴ°�?? (-1) <- �̷� ���� ���͵� �ߵǴ°�?
	{
		if (*str >= '0' && *str <= '9')
		{
			if (prev == ����) cout << "������ �ǿ����� ���� ��ġ ����";

			do
			{
				result[num++] = *(str++);
			} while (*str >= '0' && *str <= '9');

			result[num++] = ' ';
			prev = ����;
		}

		switch (*str)
		{
		case '-':
			if (prev != ����)
			{
				oper.push('~');
				prev = ���׿�����;
				break;
			}
		case '+':
		case '*':
		case '/':
		case '%':
		case '^':
			if (prev != ����) cout << "���׿����� ����";
			while (�켱����(oper.check()) >= �켱����(*str))
			{
				result[num++] = oper.pop();
				result[num++] = ' ';
			}
			oper.push(*str);
			prev = ���׿�����;
			break;
		case '(':
			if (prev == ����) cout << "����";
			oper.push(*str);
			��ȣī���� += 1;
			break;
		case ')':
			if (prev != ����) cout << "����";
			if (��ȣī���� == 0) cout << "��ȣ ����";
			char temp;
			while (true)
			{
				temp = oper.pop();
				if (temp == '(') break;
				result[num++] = temp;
				result[num++] = ' ';
			}
			��ȣī���� -= 1;
			break;
		case ' ':
			break;
		case '\0':
			if (prev != ����) cout << "����";
			if (��ȣī���� != 0) cout << "��ȣ ����";
			while (true)
			{
				temp = oper.pop();
				result[num++] = temp;
				if (temp == NULL)
				{
					return result;
				}
				else
				{
					result[num++] = ' ';
				}
			}
			break;
		default:
			cout << "�̻��� �� �־ ����";
			break;
		}

		str += 1;
	}
}

double calc(char *str)
{
	stack<double> num;
	double left, right;

	while (*str)
	{
		if (*str >= '0' && *str <= '9')
		{
			//���� ���� ������ ���� �а� ���ؿ� �ֱ�

			num.push(atof(str));
			do {
				str += 1;
			} while (*str >= '0' && *str <= '9');
		}
		else if (*str - ' ')
		{
			//���� ���� ������ ������ ������ or NULL
			//<������>
			//���ؿ��� �ǿ����� ������ ����ϰ� (�������� ���� �˻�)
			//����� ���ؿ� �ֱ�
			right = num.pop();
			if (*str != '~')
			{
				left = num.pop();
			}

			switch (*str)
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
				if (right == 0) exit(-1);//�����������������
				num.push(left / right);
				break;
			case '%':
				if (left - (int)left)
				{
					exit(-1);//�����������������
				}
				if (right - (int)right)
				{
					exit(-1);//��������������
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
		str += 1;
	}

	return num.pop();
}

int main()
{
	char input[256];
	
	cin.getline(input, 256);

	cout << "�������� " << calculate(input) << endl;
	cout << "������� " << calc(calculate(input)) << endl;

	return 0;
}
