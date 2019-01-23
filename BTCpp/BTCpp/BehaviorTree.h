#pragma once
#include "TaskStatus.h"

class Node;
class BehaviorTree
{
public:
	BehaviorTree();
	~BehaviorTree();

	void SetRuningNode(int nodeId);

private:
	int RunningNode = 0;
};

