#include <iostream>

#include <renderer/vk_engine.h>
#include <script/ScriptingMachine.h>

#include <entt.hpp>

#include <components/Transform.h>
#include <components/Camera.h>
#include <flecs.h>

int main() {
	printf("dupa\n");

	VulkanEngine engine;

	entt::registry reg;
	flecs::world ecs(0, 0);

	auto camEntity = ecs.entity("Camera");
	camEntity.set<Transform>({glm::mat4(1.f)});
	camEntity.add<Camera>();

	//wrenpp::VM vm{};
	//Vector3::bind_methods(vm);
	//vm.executeModule("assets/scripts/core/math");

	engine.init(ecs);

	engine.run(ecs);

	engine.cleanup();

	return 0;
}
