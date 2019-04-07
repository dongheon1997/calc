#pragma once

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
	T peek() const;
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

	if (--top == capacity / 4 && capacity != 8)
	{
		resize(false);
	}

	return arr[top];
}

template <typename T>
T stack<T>::peek() const
{
	if (top == 0) return NULL;

	return arr[top - 1];
}
