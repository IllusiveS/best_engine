//
// Created by patwys on 5/9/2021.
//

#ifndef BEST_ENGINE_BOX_COLLISION_H
#define BEST_ENGINE_BOX_COLLISION_H

#include <glm/vec3.hpp>

struct BoxCollisionComponent
{
	glm::vec3 min;
	glm::vec3 max;

	BoxCollisionComponent(glm::vec3 min = glm::vec3{-1.0, -1.0, -1.0}, glm::vec3 max = glm::vec3{1.0, 1.0, 1.0})
		:min(min), max(max)
	{

	};
};


#endif //BEST_ENGINE_BOX_COLLISION_H
