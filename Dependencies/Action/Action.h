#pragma once
#include <vector>
template<class ... T> class Action;
template<>
class Action<>
{
public:
	std::vector<void(*)()> pointers;
	void Call()
	{
		for (int i = 0; i < pointers.size(); i++)
			pointers[i]();
	}
	void operator +=(void(*func)())
	{
		pointers.push_back(func);
	}
};

template<class T0>
class Action<T0>
{
public:
	std::vector<void(*)(T0)> pointers;
	void Call(T0 p0)
	{
		for (int i = 0; i < pointers.size(); i++)
			pointers[i](p0);
	}
	void operator +=(void(*func)(T0))
	{
		pointers.push_back(func);
	}
};

template<class T0, class T1>
class Action<T0, T1>
{
public:
	std::vector<void(*)(T0, T1)> pointers;
	void Call(T0 p0, T1 p1)
	{
		for (int i = 0; i < pointers.size(); i++)
			pointers[i](p0, p1);
	}
	void operator +=(void(*func)(T0, T1))
	{
		pointers.push_back(func);
	}
};

template<class T0, class T1, class T2>
class Action<T0, T1, T2>

{
public:
	std::vector<void(*)(T0, T1, T2)> pointers;
	void Call(T0 p0, T1 p1, T2 p2)
	{
		for (int i = 0; i < pointers.size(); i++)
			pointers[i](p0, p1, p2);
	}
	void operator +=(void(*func)(T0, T1, T2))
	{
		pointers.push_back(func);
	}
};

