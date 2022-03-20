#pragma once

#include "taskflow.hpp"

#include "TaskBuilder.h"

struct Taskmaster
{
public:
	static Taskmaster* Get();
	static Taskmaster* _taskmaster;

	Taskmaster(const int threads);

	tf::Executor executor;

	int worldThreads{0};

	tf::CriticalSection criticalSectionFilesystem{ 1 };
	tf::CriticalSection criticalSectionImmideateSubmit{ 1 };
	tf::CriticalSection criticalSectionLongJob{ 2 };
};
