//
// Created by patwys on 5/8/2021.
//

#ifndef BEST_ENGINE_SCRIPTINGMACHINE_H
#define BEST_ENGINE_SCRIPTINGMACHINE_H

#include <wren.hpp>
#include <iostream>



class ScriptingMachine {
	WrenVM * vm;
	WrenConfiguration config;

public:
	void init();

private:
	static void writeFn(WrenVM* vm, const char* text);

	static void errorFn(WrenVM* vm, WrenErrorType errorType,
	             const char* module, const int line,
	             const char* msg);
};


#endif //BEST_ENGINE_SCRIPTINGMACHINE_H
