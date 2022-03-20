#pragma once

#include "CollisionDetectionSystem.h"
#include <Wrenpp.h>

void CollisionDetectionSystem::bindWren(wrenpp::VM vm) {
	vm.beginModule("core")
		vm.beginModule("collision")
			vm.beginModule("raycast")
				.bindClass<CollisionDetectionSystem>("CollisionDetectionSystem")
					.bindMethod< decltype(&CollisionDetectionSystem::generateRayFromCamera), &CollisionDetectionSystem::generateRayFromCamera>(false, "generateRayFromCamera()")
					.bindMethod< decltype(&CollisionDetectionSystem::raycastSingleHit), &CollisionDetectionSystem::raycastSingleHit>(false, "raycastSingleHit()")
				.endClass()
			.endModule();
		.endModule();
	.endModule();
}
