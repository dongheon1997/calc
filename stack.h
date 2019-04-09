#pragma once

template <typename T>
class Stack
{
private:
	int top;
	int capacity;
	T *data;

public:
	Stack();
	~Stack();
	void resize(bool b);
	void push(T t);
	T pop();
	T peek() const;
	bool isEmpty() const;
};

template <typename T>
Stack<T>::Stack()
{
	top = -1;
	capacity = 8;
	data = new T[capacity];
}

template <typename T>
Stack<T>::~Stack()
{
	delete[] data;
}

template <typename T>
void Stack<T>::resize(bool b)
{
	b ? capacity *= 2 : capacity /= 2;

	T *temp = new T[capacity];

	for (int i = 0; i <= top; i += 1)
	{
		temp[i] = data[i];
	}

	delete[] data;

	data = temp;
}

template <typename T>
void Stack<T>::push(T t)
{
	if (++top == capacity)
	{
		resize(true);
	}

	data[top] = t;
}

template <typename T>
T Stack<T>::pop()
{
	if (isEmpty()) throw -1;

	if (top == capacity / 4 && capacity != 8)
	{
		resize(false);
	}

	return data[top--];
}

template <typename T>
T Stack<T>::peek() const
{
	if (isEmpty()) throw -1;

	return data[top];
}

template <typename T>
inline bool Stack<T>::isEmpty() const
{
	return top == -1;
}
