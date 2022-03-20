#include "ShaderCompiler.h"

#include <vector>
#include <iostream>

#include "shaderc/shaderc.hpp"

#include <Tracy.hpp>

// Compiles a shader to a SPIR-V binary. Returns the binary as
// a vector of 32-bit words.
std::vector<uint32_t> ShaderCompilator::compile_file(const std::string& source_name,
	shaderc_shader_kind kind,
	const std::string& source,
	bool optimize)
{
	ZoneScoped;
	shaderc::Compiler compiler;
	shaderc::CompileOptions options;

	// Like -DMY_DEFINE=1
	options.AddMacroDefinition("MY_DEFINE", "1");
	if(optimize) options.SetOptimizationLevel(shaderc_optimization_level_size);

	shaderc::SpvCompilationResult module =
		compiler.CompileGlslToSpv(source_name, kind, source.c_str(), options);

	if(module.GetCompilationStatus() != shaderc_compilation_status_success)
	{
		std::cerr << module.GetErrorMessage() << std::endl;
		return std::vector<uint32_t>();
	}

	return { module.cbegin(), module.cend() };
}