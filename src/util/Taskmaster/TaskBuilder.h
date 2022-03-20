#pragma once

class Taskmaster;

struct TaskBuilder
{
	TaskBuilder(Taskmaster* _taskmaster);

private:
	Taskmaster * master;
};
