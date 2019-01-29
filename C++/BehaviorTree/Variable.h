#pragma once
class IVariable
{
public:
	IVariable() = default;

};

template<class T>
class TVariable : public IVariable
{
public:
	TVariable() = default;
	~TVariable() = default;
	TVariable(TVariable&) = delete;
	TVariable &operator=(const TVariable &) = delete;

	T Value = {};
};