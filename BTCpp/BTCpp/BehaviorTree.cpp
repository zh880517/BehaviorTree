#include "stdafx.h"
#include "BehaviorTree.h"


BehaviorTree::BehaviorTree()
{
}


BehaviorTree::~BehaviorTree()
{
}

void BehaviorTree::SetRuningNode(int nodeId)
{
	RunningNode = nodeId;
}
