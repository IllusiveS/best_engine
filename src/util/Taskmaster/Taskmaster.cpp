#include "Taskmaster.h"

Taskmaster* Taskmaster::_taskmaster = nullptr;

Taskmaster* Taskmaster::Get()
{
	return _taskmaster;
}

Taskmaster::Taskmaster(const int threads)
	: executor(threads)
{
	worldThreads = threads;
	_taskmaster = this;
}