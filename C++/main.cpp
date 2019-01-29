// BTCpp.cpp: 定义控制台应用程序的入口点。
//

#include <vector>
#include <map>
#include "BehaviorTree/TaskStatus.h"
#include "BehaviorTree/SharedVariable.h"
#include "BehaviorTree/Blackboard.h"
int main()
{
	Blackboard blackboard;
	TVariable<bool>* val = blackboard.Add<bool>("test", true);
	SharedVariable<bool> var(val);
	var = true;
    return 0;
}

