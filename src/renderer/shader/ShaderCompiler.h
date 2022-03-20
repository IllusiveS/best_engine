#pragma once

#include "shaderc/shaderc.hpp"

struct ShaderCompilator
{
	static std::vector<uint32_t> compile_file(const std::string& source_name,
		shaderc_shader_kind kind,
		const std::string& source,
		bool optimize = false);
};