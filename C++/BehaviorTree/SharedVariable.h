#pragma once
#include "Variable.h"
template<class T>
class SharedVariable
{
public:
	SharedVariable(TVariable<T>* val) :Ptr(val){}

	void Set(TVariable<T>* val) { Ptr = val; }

	T& operator=(const T& c)
	{
		if (Ptr == nullptr)
		{
			Value = c;
			return Value;
		}
		Ptr->Value = c;
		return Ptr->Value;
	}
	const T& operator()()const
	{
		if (Ptr == nullptr)
			return Value;
		return Ptr->Value;
	}
protected:
	T Value;
	TVariable<T>* Ptr;
};