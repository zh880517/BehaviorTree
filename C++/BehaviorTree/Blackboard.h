#pragma once
#include <unordered_map>
#include <string>
#include "Variable.h"
class Blackboard
{
public:
	Blackboard() = default;
	~Blackboard() 
	{
		for (auto& kv : Variables)
			delete kv.second;
	}
	
	template<class T>
	TVariable<T>* Add(const std::string& key, const T& val);

	template<class T>
	TVariable<T>* Get(const std::string& key);

private:
	std::unordered_map<std::string, IVariable*> Variables;
};

template<class T>
inline TVariable<T>* Blackboard::Add(const std::string & key, const T & val)
{
	auto it = Variables.find(key);
	if (it == Variables.end())
	{
		TVariable<T>* variable = new TVariable<T>();
		variable->Value = val;
		Variables[key] = variable;
		it = Variables.find(key);
	}
	return static_cast<TVariable<T>*>(it->second);
}

template<class T>
inline TVariable<T>* Blackboard::Get(const std::string & key)
{
	auto it = Variables.find(key);
	if (it == Variables.end())
		return nullptr;
	return static_cast<TVariable<T>*>(it->second);
}
