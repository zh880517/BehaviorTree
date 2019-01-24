#include "BehaviorTree.h"
#include "Node.h"
#include "Action.h"
#include "Conditional.h"
#include "ConditionalEvaluator.h"
#include "Decorator.h"
#include "Composite.h"

BehaviorTree::BehaviorTree()
{
}


BehaviorTree::~BehaviorTree()
{
}

TaskStatus BehaviorTree::GetNodeRunStatus(int nodeId)
{
	return TaskStatus();
}

void BehaviorTree::SetRuningNode(int nodeId)
{
	RunningNode = nodeId;
}

void BehaviorTree::OnNodeStatusChange(Node * node, TaskStatus status)
{
	
}

TaskStatus BehaviorTree::RunNode(Node * node)
{
	TaskStatus oldStatus = GetNodeRunStatus(node->GetId());
	if (oldStatus == TaskStatus::Inactive)
		node->OnAwake();
	TaskStatus status = TaskStatus::Inactive;
	if (node->GetType() <= NodeType::Conditional)
	{
		status = RunChildNode(node);
	}
	else
	{
		status = RunParentNode(static_cast<ParentNode*>(node));
	}
	switch (node->GetType())
	{
	case NodeType::Action:
	case NodeType::Conditional:
		break;
	case NodeType::Decorator:
	case NodeType::Composite:
	default:
		break;
	}
	OnNodeStatusChange(node, status);
	if (status != TaskStatus::Running)
		node->OnEnd();
	return status;
}

TaskStatus BehaviorTree::RunChildNode(Node * node)
{
	TaskStatus status = node->Update();
	if (status == TaskStatus::Running)
		SetRuningNode(node->GetId());
	return status;
}

TaskStatus BehaviorTree::RunParentNode(ParentNode * node)
{
	TaskStatus status = TaskStatus::Failure;
	int lastIndex = -1;
	while (node->CanExecute() && status != TaskStatus::Running)
	{
		int index = node->CurrentChildIndex();
		//装饰器每次获取到的索引都是0
		if (index == lastIndex)
			break;

		Node* child = node->GetChild(index);
		if (child == nullptr)
			break;
		status = RunNode(child);
		node->OnChildExecuted(index, status);
	}
	return node->OverrideStatus(status);
}

