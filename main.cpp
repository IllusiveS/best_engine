#include <iostream>

#include <renderer/vk_engine.h>
#include <script/ScriptingMachine.h>

#include <entt.hpp>

#include <components/Transform.h>
#include <components/Camera.h>

int main() {
	VulkanEngine engine;

	entt::registry reg;

	wrenpp::VM vm{};
	Vector3::bind_methods(vm);
	vm.executeModule("../assets/scripts/core/math");

	auto camera_ent = reg.create();
	reg.emplace<Transform>(camera_ent, Transform{glm::mat4(1.f)});
	reg.emplace<Camera>(camera_ent);

	ScriptingMachine mach = {};
	mach.init();
	Script test = Script::read_from_file("test");

	mach.execute_script(test);

	engine.init();

	engine.run(reg);

	engine.cleanup();

	return 0;
}
