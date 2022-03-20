#pragma once

#include "flecs.h"
#include "Ray.h"

#include "glm/vec2.hpp"

#include "script/ScriptDefs.h"

//SCRIPT_CLASS\n(?:class|struct) (\w+\n){(\n(?:\n|.)*?\n)};
SCRIPT_CLASS("core/collision/raycast")
class CollisionDetectionSystem
{
public:
	//SCRIPT_FUNCTION\n(.*) (.*)(\(.*\));
	SCRIPT_FUNCTION
	static Ray generateRayFromCamera(flecs::world& world, glm::vec2 screenPosNorm);

	SCRIPT_FUNCTION
	static flecs::entity raycastSingleHit(flecs::world& world, const Ray& ray);

	static flecs::entity raycastSingleBox(flecs::world& world, const Ray& ray);
}; 
