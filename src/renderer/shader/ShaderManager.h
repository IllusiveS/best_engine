#pragma once

#include <future>
#include <map>
#include <string>
#include <optional>
#include <vector>
#include <shared_mutex>

#include "taskflow.hpp"

#include "util/Resource.h"

#include "Shader.h"

class ShaderManager
{
public:
	static ShaderManager* Get();

	virtual void loadShader(const std::string& projectPath, const std::string& resourceId, ShaderType type) = 0;
	virtual std::optional<Resource<Shader>> getShader(const std::string& projectPath) = 0;

	virtual void Wait() = 0;

protected:
	static ShaderManager* instance;
};

class ShaderManagerMainThread : protected ShaderManager
{
public:
	//ShaderManager INTERFACE
	void loadShader(const std::string& projectPath, const std::string& resourceId, ShaderType type) override;
	std::optional<Resource<Shader>> getShader(const std::string& projectPath) override;
	//ShaderManager END

	void Setup();
	void Wait() override;

protected:
	std::shared_mutex mut;

	void AddShader(ResourceShared<Shader> resource);

	std::vector<ResourceShared<Shader>*> loadedShaders;
	std::vector<tf::Future<std::optional<Resource<Shader>>>> compilingFutures;
};
