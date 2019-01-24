#pragma once
#include <stdint.h>
enum class NodeType : uint8_t
{
	Action,
	Conditional,
	Decorator,
	Composite,
};
