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

int 우선순위(char c)
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

enum {이항연산자, 단항연산자, 숫자};

char *calculate(char *str) // 후위식으로 변경하는 함수
{
	static char result[256];
	int num = 0;

	stack<char> oper;

	int prev = 이항연산자;
	int 괄호카운터 = 0;

	while (true) // 빈 값이 들어왔을때도 잘되는가?? (-1) <- 이런 값이 들어와도 잘되는가?
	{
		if (*str >= '0' && *str <= '9')
		{
			if (prev == 숫자) cout << "중위식 피연산자 연속 배치 에러";

			do
			{
				result[num++] = *(str++);
			} while (*str >= '0' && *str <= '9');

			result[num++] = ' ';
			prev = 숫자;
		}

		switch (*str)
		{
		case '-':
			if (prev != 숫자)
			{
				oper.push('~');
				prev = 단항연산자;
				break;
			}
		case '+':
		case '*':
		case '/':
		case '%':
		case '^':
			if (prev != 숫자) cout << "이항연산자 에러";
			while (우선순위(oper.check()) >= 우선순위(*str))
			{
				result[num++] = oper.pop();
				result[num++] = ' ';
			}
			oper.push(*str);
			prev = 이항연산자;
			break;
		case '(':
			if (prev == 숫자) cout << "에러";
			oper.push(*str);
			괄호카운터 += 1;
			break;
		case ')':
			if (prev != 숫자) cout << "에러";
			if (괄호카운터 == 0) cout << "괄호 에러";
			char temp;
			while (true)
			{
				temp = oper.pop();
				if (temp == '(') break;
				result[num++] = temp;
				result[num++] = ' ';
			}
			괄호카운터 -= 1;
			break;
		case ' ':
			break;
		case '\0':
			if (prev != 숫자) cout << "에러";
			if (괄호카운터 != 0) cout << "괄호 에러";
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
			cout << "이상한 걸 넣어서 에러";
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
			//공백 만날 때까지 숫자 읽고 스텍에 넣기

			num.push(atof(str));
			do {
				str += 1;
			} while (*str >= '0' && *str <= '9');
		}
		else if (*str - ' ')
		{
			//공백 만날 때까지 읽으면 연산자 or NULL
			//<연산자>
			//스텍에서 피연산자 꺼내서 계산하고 (나눗셈은 에러 검사)
			//결과값 스텍에 넣기
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
				if (right == 0) exit(-1);//에어러러러러러러러러
				num.push(left / right);
				break;
			case '%':
				if (left - (int)left)
				{
					exit(-1);//에어러러러러러러러러
				}
				if (right - (int)right)
				{
					exit(-1);//에러러러러러러
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

	cout << "후위식은 " << calculate(input) << endl;
	cout << "결과값은 " << calc(calculate(input)) << endl;

	return 0;
}
