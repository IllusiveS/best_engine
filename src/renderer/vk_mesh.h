//
// Created by patwys on 5/7/2021.
//

#ifndef BEST_ENGINE_VK_MESH_H
#define BEST_ENGINE_VK_MESH_H


#include "vk_types.h"
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include "vk_buffer.h"

#include <tiny_obj_loader.h>

struct VertexInputDescription {

	std::vector<VkVertexInputBindingDescription> bindings;
	std::vector<VkVertexInputAttributeDescription> attributes;

	VkPipelineVertexInputStateCreateFlags flags = 0;
};


struct Vertex {

	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 uv;

	static VertexInputDescription get_vertex_description();
};


struct Mesh {
	std::vector<Vertex> _vertices;

	AllocatedBuffer _vertexBuffer;

	tinyobj::attrib_t attrib;

	//shapes contains the info for each separate object in the file
	std::vector<tinyobj::shape_t> shapes;
	//materials contains the information about the material of each shape, but we won't use it.
	std::vector<tinyobj::material_t> materials;

	bool parse_data();
	bool load_from_file(const char* filename);
};


#endif //BEST_ENGINE_VK_MESH_H
