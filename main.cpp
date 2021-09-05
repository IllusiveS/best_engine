#include <iostream>

#include <renderer/vk_engine.h>
#include <script/ScriptingMachine.h>

#include <entt.hpp>

#include <components/Transform.h>
#include <components/Camera.h>
#include <flecs.h>

#include <input_manager/input_manager.h>

int main() {
	VulkanEngine engine;

	entt::registry reg;
	flecs::world ecs(0, 0);

	auto camEntity = ecs.entity("Camera");
	camEntity.set<Transform>({ glm::translate(glm::mat4(1.f), glm::vec3{ 0.f,-6.f,-10.f }) });
	camEntity.add<Camera>();

	InputManager inputManager;

	ecs.set<InputManager>(inputManager);

	//wrenpp::VM vm{};
	//Vector3::bind_methods(vm);
	//vm.executeModule("assets/scripts/core/math");

	engine.init(ecs);

	engine.run(ecs);

	engine.cleanup();

	return 0;
}
