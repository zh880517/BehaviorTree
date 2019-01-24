#pragma once
#include <map>
#include <string>
class Blackboard
{
public:
	Blackboard();
	~Blackboard();
	
	std::map<std::string, bool>::iterator Set(const std::string& key, const bool& val);
	std::map<std::string, bool>::iterator Get(const std::string& key);

private:
	std::map<std::string, bool> bools;
	std::map<std::string, int> ints;
	std::map<std::string, float> floats;
	std::map<std::string, double> doubles;
	std::map<std::string, std::string> strings;
};

