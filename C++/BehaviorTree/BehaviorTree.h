#pragma once
#include "TaskStatus.h"
#include <vector>
#include <map>
class Node;
class ParentNode;
struct ReevaluateNodeInfo
{
	int RootNode;
	int RootChildIndex;
};
class BehaviorTree
{
public:
	BehaviorTree();
	~BehaviorTree();

public:
	TaskStatus GetNodeRunStatus(int nodeId);
	void Update();
protected:
	void BuildTreeInfo();
	void BuildChildInfo(Node* node);
	TaskStatus OnUpdate();
	TaskStatus ReevaluateNode();
	void AbortByChild(ParentNode* parent, int childIndex);
	int GetNodeParentId(int nodeId);
	Node* GetNode(int nodeId);
	ParentNode* GetParent(int nodeId);
	void SetRuningNode(int nodeId);
	void OnNodeStatusChange(Node* node, TaskStatus status);
	TaskStatus RunNode(Node* node);
	TaskStatus OnNodeComplete(Node* node, TaskStatus status);
	TaskStatus RunChildNode(Node* node);
	TaskStatus RunParentNode(ParentNode* node);
private:
	std::vector<TaskStatus> NodeRunStatus;
	std::vector<int> NodeParentId;
	std::vector<Node*> Nodes;
	std::vector<int> NodeInParentIndex;
	std::map<int, ReevaluateNodeInfo> ReevaluateNodeParent;
	std::vector<int> WaiteReevaluateNode;
	Node* Root = nullptr;
	int RunningNode = -1;
	bool ReStartWhenComplete = false;
	bool CanUpdate = false;
};

