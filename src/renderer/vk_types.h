//
// Created by patwys on 5/5/2021.
//

#ifndef BEST_ENGINE_VK_TYPES_H
#define BEST_ENGINE_VK_TYPES_H

#include <vulkan/vulkan.h>
#include <iostream>

#include "vk_mem_alloc.h"

//we want to immediately abort when there is an error. In normal engines this would give an error message to the user, or perform a dump of state.

#define VK_CHECK(x)                                                 \
	do                                                              \
	{                                                               \
		VkResult err = x;                                           \
		if (err)                                                    \
		{                                                           \
			std::cout <<"Detected Vulkan error: " << err << std::endl; \
			abort();                                                \
		}                                                           \
	} while (0)



#endif //BEST_ENGINE_VK_TYPES_H
