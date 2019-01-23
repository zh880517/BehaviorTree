#pragma once
#include <stdint.h>
enum class NodeType : uint8_t
{
	Action,
	Conditional,
	ConditionalDecorator,
	Decorator,
	Composite,
};
