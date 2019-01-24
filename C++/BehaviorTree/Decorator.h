#pragma once
#include "ParentNode.h"
class Decorator : public ParentNode
{
public:
	Decorator(BehaviorTree* owner, int id) :
		ParentNode(owner, NodeType::Decorator, id)
	{
	}

	virtual void AddChild(Node* node) override
	{
		Child = node;
	}

	virtual bool HasChildren() const override
	{
		return Child != nullptr;
	}

	virtual Node* GetChild(int childIndex) override
	{
		return childIndex == 0 ? Child : nullptr;
	}

	virtual bool CanExecute()
	{
		return ChildStatus == TaskStatus::Inactive || ChildStatus == TaskStatus::Running;
	}

	virtual void OnChildExecuted(int childIndex, TaskStatus childStatus) override
	{
		ChildStatus = childStatus;
	}

	virtual void OnEnd() override
	{
		ChildStatus = TaskStatus::Inactive;
	}

protected:
	Node * Child = nullptr;
	TaskStatus ChildStatus = TaskStatus::Inactive;
};
