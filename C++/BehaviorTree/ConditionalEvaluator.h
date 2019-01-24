#pragma once
#include "ParentNode.h"
#include "Decorator.h"
#include "Conditional.h"
class ConditionalEvaluator : public Decorator
{
public:
	ConditionalEvaluator(BehaviorTree* owner, int id) :
		Decorator(owner, id)
	{
	}

	~ConditionalEvaluator() {
		if (ConditionalNode != nullptr)
			delete ConditionalNode;
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
		Decorator::OnEnd();
		CheckConditionalTask = true;
		ConditionalTaskFailed = false;
		if (ConditionalNode != nullptr)
			ConditionalNode->OnEnd();
	}

	virtual bool CanExecute() override
	{
		if (CheckConditionalTask) {
			CheckConditionalTask = false;
			Update();
		}

		if (ConditionalTaskFailed) {
			return false;
		}
		return ChildStatus == TaskStatus::Inactive || ChildStatus == TaskStatus::Running;
	}

	virtual TaskStatus Update() override
	{
		ConditionalTaskFailed = false;
		if (ConditionalNode != nullptr)
		{
			CheckConditionalTask = false;
			auto childStatus = ConditionalNode->Update();
			ConditionalTaskFailed = childStatus == TaskStatus::Failure;
			return childStatus;
		}
		return TaskStatus::Failure;
	}

private:
	Conditional* ConditionalNode = nullptr;
	bool CheckConditionalTask = true;
	bool ConditionalTaskFailed = false;
};
