#pragma once
#include "ParentNode.h"
#include "Conditional.h"

class ConditionalDecorator : public ParentNode
{
public:
	ConditionalDecorator(BehaviorTree* owner, int id) :
		ParentNode(owner, NodeType::ConditionalDecorator, id)
	{
	}

	void SetConditional(Conditional* conditional)
	{
		ConditionalNode = conditional;
	}

	virtual void OnAwake()
	{
		if (ConditionalNode != nullptr)
		{
			ConditionalNode->OnAwake();
		}
	}

	virtual void OnEnd() override
	{
		ChildStatus = TaskStatus::Inactive;
		CheckConditionalTask = true;
		ConditionalTaskFailed = false;
		if (ConditionalNode != nullptr)
			ConditionalNode->OnEnd();
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

	virtual void OnChildExecuted(int childIndex, TaskStatus childStatus) override
	{
		ChildStatus = childStatus;
	}

	virtual bool CanExecute() override
	{
		if (CheckConditionalTask) {
			CheckConditionalTask = false;
			ConditionalTaskFailed = false;
			if (ConditionalNode != nullptr)
			{
				auto childStatus = ConditionalNode->Update();
				ConditionalTaskFailed = childStatus == TaskStatus::Failure;
			}
		}

		if (ConditionalTaskFailed) {
			return false;
		}
		return ChildStatus == TaskStatus::Inactive || ChildStatus == TaskStatus::Running;
	}

private:
	Conditional* ConditionalNode = nullptr;
	Node * Child;
	TaskStatus ChildStatus = TaskStatus::Inactive;
	bool CheckConditionalTask = true;
	bool ConditionalTaskFailed = false;
};
