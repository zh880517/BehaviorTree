#pragma once
#include "Node.h"
class Action : public Node
{
public:
	Action(BehaviorTree* owner, int id) :
		Node(owner, NodeType::Action, id)
	{
	}

};

