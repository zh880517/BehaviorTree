#pragma once
#include <stdint.h>
enum class TaskStatus : uint8_t
{
	Inactive,
	Failure,
	Success,
	Running
};

