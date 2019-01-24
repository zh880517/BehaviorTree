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
	for (auto& node :Nodes)
		delete node;
}

TaskStatus BehaviorTree::GetNodeRunStatus(int nodeId)
{
	TaskStatus status = TaskStatus::Inactive;
	if (nodeId >= 0 && nodeId < (int)NodeParentId.size())
		status = NodeRunStatus[nodeId];
	return status;
}

void BehaviorTree::Update()
{
	if (!CanUpdate)
		return;
	TaskStatus status = OnUpdate();
	if (status != TaskStatus::Running)
	{
		//此时整棵树已经运行完成
		CanUpdate = ReStartWhenComplete;
	}
}

TaskStatus BehaviorTree::OnUpdate()
{
	if (RunningNode < 0)
		return RunNode(Root);
	Node* node = GetNode(RunningNode);
	return RunNode(node);
}

int BehaviorTree::GetNodeParentId(int nodeId)
{
	int id = -1;
	if (nodeId >= 0 && nodeId < (int)NodeParentId.size())
		id = NodeParentId[nodeId];
	return id;
}

Node * BehaviorTree::GetNode(int nodeId)
{
	if (nodeId >= 0 && nodeId < (int)Nodes.size())
		return Nodes[nodeId];
	return nullptr;
}

ParentNode * BehaviorTree::GetParent(int nodeId)
{
	int parentId = GetNodeParentId(nodeId);
	if (parentId >= 0)
	{
		Node* parent = GetNode(parentId);
		if (parent != nullptr)
			return static_cast<ParentNode*>(parent);
	}
	return nullptr;
}

void BehaviorTree::SetRuningNode(int nodeId)
{
	RunningNode = nodeId;
}

void BehaviorTree::OnNodeStatusChange(Node * node, TaskStatus status)
{
	NodeRunStatus[node->GetId()] = status;
	if (status != TaskStatus::Running && node->GetId() == RunningNode)
		RunningNode = -1;
}

TaskStatus BehaviorTree::RunNode(Node * node)
{
	TaskStatus oldStatus = GetNodeRunStatus(node->GetId());
	if (oldStatus != TaskStatus::Running)
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
	OnNodeStatusChange(node, status);
	if (status == TaskStatus::Failure || status == TaskStatus::Success)
		return OnNodeComplete(node, status);
	return status;
}

TaskStatus BehaviorTree::OnNodeComplete(Node * node, TaskStatus status)
{
	node->OnEnd();
	ParentNode* parentNode = GetParent(node->GetId());
	if (parentNode != nullptr)
	{
		auto it = NodeInParentIndex.find(node->GetId());
		if (it != NodeInParentIndex.end())
		{
			parentNode->OnChildExecuted(it->second, status);
			status = parentNode->OverrideStatus(status);
			if (status == TaskStatus::Running)
				return TaskStatus::Running;
			if (parentNode->CanExecute())
				return RunNode(parentNode);
			return OnNodeComplete(parentNode, status);
		}
	}
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
	if (node->CanExecute())
	{
		int index = node->CurrentChildIndex();
		Node* child = node->GetChild(index);
		if (child != nullptr)
		{
			NodeInParentIndex[node->GetId()] = index;
			return RunNode(child);
		}
	}
	return TaskStatus::Failure;
}

