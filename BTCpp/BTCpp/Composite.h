#pragma once
#include <vector>
#include "ParentNode.h"

class Composite : ParentNode
{
public:
	Composite(BehaviorTree* owner, int id) :
		ParentNode(owner, NodeType::Action, id)
	{
	}
	virtual void OnAwake() override 
	{
		if (Children.size() != ChildrenStatus.size())
			ChildrenStatus.resize(Children.size(), TaskStatus::Inactive);
	}

	virtual void OnEnd() override
	{
		CurrentIndex = 0;
		for (auto & st : ChildrenStatus)
			st = TaskStatus::Inactive;
	}

	virtual void AddChild(Node* node) override
	{
		Children.push_back(node);
	}

	virtual bool HasChildren() const override
	{
		return !Children.empty();
	}

	virtual Node* GetChild(int childIndex) override
	{
		return childIndex >= 0 && childIndex < Children.size() ? Children[childIndex] : nullptr;
	}

	virtual void OnChildExecuted(int childIndex, TaskStatus childStatus) override
	{
		++CurrentIndex;
		if (childIndex >=0 && childIndex < ChildrenStatus.size())
		{
			ChildrenStatus[childIndex] = childStatus;
		}
	}

	virtual void OnConditionalAbort(int childIndex)
	{
		CurrentIndex = childIndex;
		for (int i = childIndex; i < ChildrenStatus.size(); ++i)
			ChildrenStatus[i] = TaskStatus::Inactive;
	}

protected:
	int CurrentIndex = 0;
	std::vector<Node*> Children;
	std::vector<TaskStatus> ChildrenStatus;
};
