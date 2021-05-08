#include <iostream>

#include <renderer/vk_engine.h>
#include <script/ScriptingMachine.h>

int main() {
	VulkanEngine engine;

	ScriptingMachine mach = {};
	mach.init();

	engine.init();

	engine.run();

	engine.cleanup();

	return 0;
}
