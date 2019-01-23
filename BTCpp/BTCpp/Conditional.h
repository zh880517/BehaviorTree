#pragma once
#include "Node.h"
#include "BehaviorTree.h"
class Conditional : public Node
{
public:
	Conditional(BehaviorTree* owner, int id) :
		Node(owner, NodeType::Action, id)
	{
	}

};
