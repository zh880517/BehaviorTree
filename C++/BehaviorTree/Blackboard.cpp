#include "Blackboard.h"
#include "SharedVariable.h"


Blackboard::Blackboard()
{
}


Blackboard::~Blackboard()
{
}

std::map<std::string, bool>::iterator Blackboard::Set(const std::string & key, const bool & val)
{
	auto it = bools.find(key);
	if (it == bools.end())
		it = bools.insert(std::pair<std::string, bool>(key, val)).first;
	else
		it->second = val;
	SharedVariable<bool> test(it);
	return it;
}

std::map<std::string, bool>::iterator Blackboard::Get(const std::string & key)
{
	return std::map<std::string, bool>::iterator();
}

