#pragma once

#include "Node.h"
class ParentNode : public Node
{
public:
	ParentNode(BehaviorTree* owner, NodeType type, int id) :
		Node(owner, type, id)
	{
	}

	virtual void AddChild(Node* node) = 0;
	virtual bool HasChildren()const = 0;
	virtual Node* GetChild(int childIndex) = 0;
	virtual bool CanExecute() = 0;
	virtual int CurrentChildIndex() { return 0; }
	virtual void OnChildExecuted(int childIndex, TaskStatus childStatus) {}


};
