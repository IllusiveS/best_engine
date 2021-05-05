//
// Created by patwys on 5/5/2021.
//

#ifndef BEST_ENGINE_VK_INIT_H
#define BEST_ENGINE_VK_INIT_H

#include "vk_types.h"

namespace vkinit {
	VkCommandPoolCreateInfo command_pool_create_info(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags flags = 0);

	VkCommandBufferAllocateInfo command_buffer_allocate_info(VkCommandPool pool, uint32_t count = 1, VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
}


#endif //BEST_ENGINE_VK_INIT_H
