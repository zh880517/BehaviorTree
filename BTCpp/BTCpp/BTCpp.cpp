// BTCpp.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <vector>
#include "TaskStatus.h"

int main()
{
	std::vector<TaskStatus> test(10, TaskStatus::Inactive);
	for (auto& st : test)
	{
		st = TaskStatus::Failure;
	}
    return 0;
}

