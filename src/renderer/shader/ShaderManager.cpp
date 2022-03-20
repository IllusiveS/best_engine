#include "ShaderManager.h"

#include <taskflow.hpp>

#include <cassert>

#include <util/Taskmaster/Taskmaster.h>
#include <util/File.h>
#include <util/md5.h>

#include "renderer/vk_engine.h"
#include "ShaderCompiler.h"

#include <Tracy.hpp>

#include "Shader.h"

ShaderManager* ShaderManager::instance;

ShaderManager* ShaderManager::Get()
{
	assert(instance != nullptr);
	return instance;
}


void ShaderManagerMainThread::Setup()
{
	instance = this;
}

void ShaderManagerMainThread::loadShader(const std::string& projectPath, const std::string& resourceId, ShaderType type)
{
	static_assert(std::is_copy_assignable_v< std::optional<Resource<Shader>>>);
	static_assert(std::is_constructible_v< std::optional<Resource<Shader>>>);
	static_assert(std::is_copy_constructible_v< std::optional<Resource<Shader>>>);
	static_assert(std::is_move_constructible_v< std::optional<Resource<Shader>>>);
	static_assert(std::is_move_assignable_v< std::optional<Resource<Shader>>>);

	tf::Future< std::optional<Resource<Shader>>> shaderFuture = Taskmaster::Get()->executor.async([path=projectPath, type, this, resId = resourceId]()
		{
			ZoneScopedN("LoadShader");
			File shaderFile { path };

			std::vector<uint32_t> byteCode;
			std::string sourceHashVal;
			VkShaderModule finalShaderModule;
			MD5 sourceHash;

			auto& executor = Taskmaster::Get()->executor;

			sourceHash.update((const unsigned char*) shaderFile.fileData, shaderFile.fileSize);
			sourceHashVal = sourceHash.hexdigest();

			std::string shaderCode{ (const char*) shaderFile.fileData, (unsigned int)shaderFile.fileSize };
			byteCode = ShaderCompilator::compile_file(shaderCode, type == ShaderType::VERTEX ? shaderc_shader_kind::shaderc_vertex_shader : shaderc_shader_kind::shaderc_fragment_shader, path);
			//std::vector<uint32_t> temp = ShaderCompilator::compile_file(shaderFile.fileData, shaderFile.fileSize , type == ShaderType::VERTEX ? shaderc_shader_kind.shaderc_vertex_shader : shaderc_shader_kind.shaderc_fragment_shader, projectPath);
			


			//UploadShaderToGPU
			//create a new shader module, using the buffer we loaded
			VkShaderModuleCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.pNext = nullptr;

			//codeSize has to be in bytes, so multply the ints in the buffer by size of int to know the real size of the buffer
			createInfo.codeSize = byteCode.size() * sizeof(uint32_t);
			createInfo.pCode = byteCode.data();

			//check that the creation goes well.
			VkShaderModule shaderModule;
			if(vkCreateShaderModule(VulkanEngine::Globals()._device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
			{
				//return false;
			}
			finalShaderModule = shaderModule;

			Shader* ptr = new Shader();
			ptr->sourceHash = sourceHashVal;
			ptr->type = type;
			ptr->name = path;
			ptr->shaderModule = finalShaderModule;


			ResourceShared<Shader>* newResShader = new ResourceShared<Shader>(ptr, path);
			newResShader->id = resId;
			newResShader->data = ptr;
			newResShader->path = path;

			std::unique_lock lock(mut);

			loadedShaders.push_back(newResShader);

			return Resource<Shader>(newResShader);
	});

	compilingFutures.push_back(std::move(shaderFuture));
}

std::optional<Resource<Shader>> ShaderManagerMainThread::getShader(const std::string& id)
{
	std::shared_lock lock(mut);

	auto result = std::find_if(
		std::begin(loadedShaders)
		, std::end(loadedShaders)
		, [&](const ResourceShared<Shader>* res) {
			return res->id == id;
		});

	if(result != std::end(loadedShaders))
	{
		return Resource<Shader>(*result);
	}

	return std::nullopt;
}

void ShaderManagerMainThread::Wait()
{
	for(auto& future : compilingFutures)
	{
		future.get();
	}
}
