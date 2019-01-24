#pragma once
#include <map>
template< class T>
class SharedVariable
{
public:
	typedef typename std::map<std::string, T>::iterator Value;
	SharedVariable() = default;
	SharedVariable(const SharedVariable<T>&) = default;
	SharedVariable(SharedVariable<T>&&) = default;
	SharedVariable(const Value& it) :It(it) {}
	T& operator=(const T& c)
	{
		It->second = c;
		return It->second;
	}

	const T& operator()()const
	{
		return It->second;
	}

private:
	Value It;
};

