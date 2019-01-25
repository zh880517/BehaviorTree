#pragma once
#include <vector>
#include "ParentNode.h"
#include "AbortType.h"

class Composite : public ParentNode
{
public:
	Composite(BehaviorTree* owner, int id) :
		ParentNode(owner, NodeType::Composite, id)
	{
	}

	AbortType GetAbortType()const { return Abort; }

	virtual void OnStart() override 
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

	virtual Node* GetChild(int childIndex) override
	{
		return childIndex >= 0 && childIndex <(int) Children.size() ? Children[childIndex] : nullptr;
	}

	virtual void OnChildExecuted(int childIndex, TaskStatus childStatus) override
	{
		++CurrentIndex;
		if (childIndex >=0 && childIndex < (int)ChildrenStatus.size())
		{
			ChildrenStatus[childIndex] = childStatus;
		}
	}

	virtual void OnConditionalAbort(int childIndex) override
	{
		CurrentIndex = childIndex;
		for (int i = childIndex; i < (int)ChildrenStatus.size(); ++i)
			ChildrenStatus[i] = TaskStatus::Inactive;
	}
	virtual int CurrentChildIndex() override
	{
		return CurrentIndex;
	}

	virtual int ChildrenCount() const override
	{
		return (int)Children.size();
	}

protected:
	std::vector<Node*> Children;
	std::vector<TaskStatus> ChildrenStatus;
	int CurrentIndex = 0;
	AbortType Abort = AbortType::None;
};
