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

void BehaviorTree::BuildTreeInfo()
{
	size_t nodeNumber = Nodes.size();
	NodeRunStatus.resize(nodeNumber, TaskStatus::Inactive);
	NodeParentId.resize(nodeNumber, -1);
	NodeInParentIndex.resize(nodeNumber, -1);
	if (Root != nullptr)
		BuildChildInfo(Root);

	//构建再评估节点信息
	for (auto node : Nodes)
	{
		if (node->GetType() == NodeType::Conditional)
		{
			ParentNode* parent = GetParent(node->GetId());
			int lastNodeId = node->GetId();
			while (parent != nullptr)
			{
				if (parent->GetType() == NodeType::Composite)
				{
					Composite* compositeNode = static_cast<Composite*>(parent);
					if (compositeNode->GetAbortType() != AbortType::None)
					{
						ReevaluateNodeInfo info;
						info.RootNode = parent->GetId();
						info.RootChildIndex = lastNodeId;
						ReevaluateNodeParent.insert(std::pair<int, ReevaluateNodeInfo>(node->GetId(), info));
					}
					break;
				}
				lastNodeId = parent->GetId();
				parent = GetParent(parent->GetId());
			}
		}
	}
}

void BehaviorTree::BuildChildInfo(Node * node)
{
	if (node->GetType() <= NodeType::Conditional)
		return;
	ParentNode* parentNode = static_cast<ParentNode*>(node);
	for (int i=0; i<parentNode->ChildrenCount(); ++i)
	{
		Node* child = parentNode->GetChild(i);
		if (child != nullptr)
		{
			NodeInParentIndex[child->GetId()] = i;
			NodeParentId[child->GetId()] = parentNode->GetId();
			BuildChildInfo(child);
		}
	}
}


TaskStatus BehaviorTree::OnUpdate()
{
	if (RunningNode < 0)
		return RunNode(Root);
	TaskStatus reevaluateStatus = ReevaluateNode();
	if (reevaluateStatus != TaskStatus::Inactive)
		return reevaluateStatus;

	Node* node = GetNode(RunningNode);
	if (node != nullptr)
		return RunNode(node);
	return TaskStatus::Inactive;
}

TaskStatus BehaviorTree::ReevaluateNode()
{
	if (!WaiteReevaluateNode.empty())
	{
		std::vector<int> needReevaluateNode(WaiteReevaluateNode.size());
		Node* reRunNode = nullptr;
		for (auto id : WaiteReevaluateNode)
		{
			TaskStatus lastStatus = GetNodeRunStatus(id);
			do 
			{
				if (lastStatus == TaskStatus::Inactive)
					break;
				auto it = ReevaluateNodeParent.find(id);
				if (it == ReevaluateNodeParent.end())
					break;
				TaskStatus parentStatus = GetNodeRunStatus(it->second.RootNode);
				if (parentStatus == TaskStatus::Inactive)
					break;
				Composite* parentNode = static_cast<Composite*>(GetNode(it->second.RootNode));
				ParentNode* grandpaNode = GetParent(parentNode->GetId());
				//如果复合节点的父节点没有处于运行状态，则无法触发中断
				if (grandpaNode != nullptr && GetNodeRunStatus(grandpaNode->GetId()) != TaskStatus::Running)
					break;
				//复合节点只有处于运行状态才可以自我打断
				if (parentStatus != TaskStatus::Running && parentNode->GetAbortType() == AbortType::Self)
					break;
				Node* node = GetNode(id);
				TaskStatus newStatus = node->Update();
				//如果状态没有改变，则等待下次再检查
				if (newStatus == lastStatus)
				{
					needReevaluateNode.push_back(id);
					break;
				}
				AbortByChild(parentNode, it->second.RootChildIndex);
				reRunNode = parentNode;
				if (parentStatus != TaskStatus::Running)
				{
					ParentNode* runningRoot = GetParent(parentNode->GetId());
					int abortChildIndex = NodeInParentIndex[parentNode->GetId()];
					AbortByChild(runningRoot, abortChildIndex);
					reRunNode = runningRoot;
				}

			} while (false);
			if (reRunNode != nullptr)
				break;
		}
		WaiteReevaluateNode.swap(needReevaluateNode);
		if (reRunNode != nullptr)
			return RunNode(reRunNode);
	}
	return TaskStatus::Inactive;
}

void BehaviorTree::AbortByChild(ParentNode * parent, int childIndex)
{
	for (int i=childIndex; i<(int)parent->ChildrenCount(); ++i)
	{
		Node* node = parent->GetChild(i);
		if (node != nullptr)
		{
			if (GetNodeRunStatus(node->GetId()) == TaskStatus::Running)
				node->OnEnd();
			OnNodeStatusChange(node, TaskStatus::Inactive);
			if (node->GetType() > NodeType::Conditional)
			{
				ParentNode* parentNode = static_cast<ParentNode*>(node);
				parentNode->OnConditionalAbort(0);
				AbortByChild(parentNode, 0);
			}
		}
	}
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
		node->OnStart();
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
	if (node->GetType() == NodeType::Conditional && ReevaluateNodeParent.find(node->GetId()) != ReevaluateNodeParent.end())
	{
		WaiteReevaluateNode.push_back(node->GetId());
	}
	node->OnEnd();
	ParentNode* parentNode = GetParent(node->GetId());
	if (parentNode != nullptr)
	{
		int childIndex = NodeInParentIndex[node->GetId()];
		if (childIndex >= 0)
		{
			parentNode->OnChildExecuted(childIndex, status);
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
			return RunNode(child);
		}
	}
	return TaskStatus::Failure;
}


