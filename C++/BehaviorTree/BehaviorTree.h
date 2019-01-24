#pragma once
#include "TaskStatus.h"
#include <vector>
#include <map>
class Node;
class ParentNode;

class BehaviorTree
{
public:
	BehaviorTree();
	~BehaviorTree();

public:
	TaskStatus GetNodeRunStatus(int nodeId);
	void Update();
protected:
	TaskStatus OnUpdate();
	int GetNodeParentId(int nodeId);
	Node* GetNode(int nodeId);
	ParentNode* GetParent(int nodeId);
	void SetRuningNode(int nodeId);
	void OnNodeStatusChange(Node* node, TaskStatus status);
	TaskStatus RunNode(Node* node);
	TaskStatus OnNodeComplete(Node* node, TaskStatus status);
	TaskStatus RunChildNode(Node* node);
	TaskStatus RunParentNode(ParentNode* node);
	/*
	TaskStatus RunConditionalDecorator(ConditionalDecorator* node);
	TaskStatus RunDecorator(Decorator* node);
	TaskStatus RunComposite(Composite* node);
	*/

private:
	std::vector<TaskStatus> NodeRunStatus;
	std::vector<int> NodeParentId;
	std::vector<Node*> Nodes;
	std::map<int, int> NodeInParentIndex;
	Node* Root = nullptr;
	int RunningNode = -1;
	bool ReStartWhenComplete = false;
	bool CanUpdate = false;
};

