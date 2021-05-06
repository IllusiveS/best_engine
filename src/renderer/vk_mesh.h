//
// Created by patwys on 5/7/2021.
//

#ifndef BEST_ENGINE_VK_MESH_H
#define BEST_ENGINE_VK_MESH_H


#include "vk_types.h"
#include <vector>
#include <glm/vec3.hpp>

struct VertexInputDescription {

	std::vector<VkVertexInputBindingDescription> bindings;
	std::vector<VkVertexInputAttributeDescription> attributes;

	VkPipelineVertexInputStateCreateFlags flags = 0;
};


struct Vertex {

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;

	static VertexInputDescription get_vertex_description();
};


struct Mesh {
	std::vector<Vertex> _vertices;

	AllocatedBuffer _vertexBuffer;
};


#endif //BEST_ENGINE_VK_MESH_H
