//
// Created by patwys on 5/8/2021.
//

#include "Script.h"

#include <fstream>
#include <vector>
#include <cassert>

Script Script::read_from_file(const std::string& script_name)
{
	auto filepath = "../assets/scripts/" + script_name + ".wren";

	std::ifstream file(filepath);

	if (!file.is_open()) {
		assert(false);
	}

	std::string script = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	//now that the file is loaded into the buffer, we can close it
	file.close();

	Script scr = {};
	scr.script = script;
	scr.scriptName = script_name;
	return scr;
}