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
	T checkTop() const;
};
