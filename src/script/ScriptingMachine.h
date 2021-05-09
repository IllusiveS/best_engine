//
// Created by patwys on 5/8/2021.
//

#ifndef BEST_ENGINE_SCRIPTINGMACHINE_H
#define BEST_ENGINE_SCRIPTINGMACHINE_H

#include <wren.hpp>
#include <iostream>
#include "Script.h"


class ScriptingMachine {
	WrenVM * vm;
	WrenConfiguration config;

public:
	void init();

	void execute_script(Script script);
private:
	static void writeFn(WrenVM* vm, const char* text);

	static void errorFn(WrenVM* vm, WrenErrorType errorType,
	             const char* module, const int line,
	             const char* msg);
};


#endif //BEST_ENGINE_SCRIPTINGMACHINE_H
