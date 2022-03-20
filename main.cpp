#include <iostream>

#include <renderer/vk_engine.h>
#include <script/ScriptingMachine.h>

#include <components/Transform.h>
#include <components/Camera.h>
#include <flecs.h>

#include <input_manager/input_manager.h>

int main() {

	

	/*ecs_entity_t ecs_id(id) = 0;

	auto ShaderDesc = ecs_component_desc_t {
		.entity = {
			.entity = ecs_id(id),
			.name = "GreenMaterial",
		},
		.size = 0,
		.alignment = 0
	};

	ecs_id(id) = ecs_component_init(ecs, &ShaderDesc);
	ecs_assert(ecs_id(id) != 0, ECS_INVALID_PARAMETER, NULL);*/

	flecs::world ecs(0, 0);

	//auto HAS_MATERIAL = ecs.entity("has_material");
	//auto meshEntity = ecs.entity("mesh");
	//auto meshEntity2 = ecs.entity("mesh1");
	//auto meshEntity3 = ecs.entity("mesh2");
	//auto materialEntity = ecs.entity("material");
	//auto materialEntity2 = ecs.entity("material1");

	//meshEntity.add(HAS_MATERIAL, materialEntity);
	//meshEntity2.add(HAS_MATERIAL, materialEntity);
	//meshEntity3.add(HAS_MATERIAL, materialEntity2);

	////std::cout << "mesh1's type: [" << meshEntity.type().str() << "]\n";
	//
	//auto query = ecs.query_builder()
	//	.term(HAS_MATERIAL).obj(materialEntity)
	//	.build();

	//query.each([](flecs::entity ent) {
	//	//std::cout << ent.name() << std::endl;
	//});

	//query.iter([](flecs::iter& iter) {
	//	std::cout << "Meshes with material1" << std::endl;

	//	for(const auto& it : iter)
	//	{
	//		std::cout << iter.entity(it).name() << std::endl;
	//	}
	//});

	//std::cout << std::endl;

	//auto query2 = ecs.query_builder()
	//	.term(HAS_MATERIAL).obj(materialEntity2)
	//	.build();

	//query2.iter([](flecs::iter& iter)
	//	{
	//		std::cout << std::endl << "Meshes with material2" << std::endl;

	//		for(const auto& it : iter)
	//		{
	//			std::cout << iter.entity(it).name() << std::endl;
	//		}
	//	});
	//return 0;

	/*materialEntity.each(HAS_MATERIAL, flecs::Wildcard, [](flecs::id id)
		{
			if(id.is_pair())
			{
				std::cout << "material relation " << id.relation().name() << " " << id.object().name() << std::endl;
			}

		});


	meshEntity.each(HAS_MATERIAL, flecs::Wildcard, [](flecs::id id) {
		if(id.is_pair())
		{
			std::cout << "mesh1 " << id.relation().name() << " " << id.object().name() << std::endl;
		}
	});*/

	

	//wrenpp::VM vm{};
	//Vector3::bind_methods(vm);
	//vm.executeModule("assets/scripts/core/math");

	VulkanEngine engine;
	engine.init();

	engine.run();

	engine.cleanup();

	return 0;
}
