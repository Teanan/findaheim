#pragma once

#include <list>

template <typename T>
class list_maker
{
	std::list<T> L;
public:
	list_maker& operator()(const T& val)
	{
		L.push_back(val);   return *this;
	}
	operator std::list<T>()
	{
		return L;
	}
};

template <typename T>
list_maker<T> make_list(const T& firstval)
{
	return list_maker<T>()(firstval);
}