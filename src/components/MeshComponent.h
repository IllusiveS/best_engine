//
// Created by patwys on 5/12/2021.
//

#ifndef BEST_ENGINE_MESHCOMPONENT_H
#define BEST_ENGINE_MESHCOMPONENT_H


#include <renderer/vk_mesh.h>
#include <renderer/vk_engine.h>

struct MeshComponent {
public:
	Mesh* _mesh;
	Material* _material;
};


#endif //BEST_ENGINE_MESHCOMPONENT_H
