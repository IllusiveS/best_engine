//
// Created by patwys on 5/8/2021.
//

#include "ScriptingMachine.h"

void ScriptingMachine::init() {
	wrenInitConfiguration(&config);
	config.writeFn = &writeFn;
	config.errorFn = &errorFn;
	vm = wrenNewVM(&config);
}

void ScriptingMachine::execute_script(Script script)
{
	WrenInterpretResult result = wrenInterpret(vm, "main", script.script.c_str());
}

void ScriptingMachine::writeFn(WrenVM* vm, const char* text)
{
	printf("%s", text);
}

void ScriptingMachine::errorFn(WrenVM* vm, WrenErrorType errorType,
             const char* module, const int line,
             const char* msg)
{
	switch (errorType)
	{
		case WREN_ERROR_COMPILE:
		{
			printf("[%s line %d] [Error] %s\n", module, line, msg);
		} break;
		case WREN_ERROR_STACK_TRACE:
		{
			printf("[%s line %d] in %s\n", module, line, msg);
		} break;
		case WREN_ERROR_RUNTIME:
		{
			printf("[Runtime Error] %s\n", msg);
		} break;
	}
}