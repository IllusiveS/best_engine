#pragma once

#include <vulkan/vulkan_core.h>

#include <string>

enum class ShaderType
{
	VERTEX
	,FRAGMENT
	,COMPUTE
};

struct Shader
{
	std::string name;
	std::string sourceHash;
	ShaderType type;
	VkShaderModule shaderModule;
};