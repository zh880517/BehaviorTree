#pragma once
#include "TaskStatus.h"

class Node;
class Action;
class Conditional;
class Composite;
class ConditionalEvaluator;
class Decorator;
class ParentNode;
class BehaviorTree
{
public:
	BehaviorTree();
	~BehaviorTree();

public:
	TaskStatus GetNodeRunStatus(int nodeId);
protected:
	void SetRuningNode(int nodeId);
	void OnNodeStatusChange(Node* node, TaskStatus status);
	TaskStatus RunNode(Node* node);
	TaskStatus RunChildNode(Node* node);
	TaskStatus RunParentNode(ParentNode* node);
	/*
	TaskStatus RunConditionalDecorator(ConditionalDecorator* node);
	TaskStatus RunDecorator(Decorator* node);
	TaskStatus RunComposite(Composite* node);
	*/

private:
	int RunningNode = 0;
};

