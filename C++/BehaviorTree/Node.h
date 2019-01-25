#pragma once
#include "NodeType.h"
#include "TaskStatus.h"
class BehaviorTree;
class Node
{
public:
	Node(BehaviorTree* owner, NodeType type, int id) :
		Id(id), Type(type), Owner(owner) 
	{
	}
	virtual ~Node() {}
	Node(Node&) = delete;
	Node &operator=(const Node &) = delete;

public:
	int GetId()const { return Id; }
	NodeType GetType()const { return Type; }
	BehaviorTree* GetOwner() { return Owner; }
	virtual TaskStatus Update() = 0;
	virtual void OnInit() {}//只调用一次
	virtual void OnStart() {};
	virtual void OnEnd() {};

private:
	int Id;
	NodeType Type;
	BehaviorTree* Owner;
};

