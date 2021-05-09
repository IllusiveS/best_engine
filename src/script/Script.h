//
// Created by patwys on 5/8/2021.
//

#ifndef BEST_ENGINE_SCRIPT_H
#define BEST_ENGINE_SCRIPT_H

#include <string>

class Script {
public:
	static Script read_from_file(const std::string& script_name);

	std::string scriptName;
	std::string script;
};


#endif //BEST_ENGINE_SCRIPT_H
