//
// Created by patwys on 5/5/2021.
//

#ifndef BEST_ENGINE_VK_TYPES_H
#define BEST_ENGINE_VK_TYPES_H

#include <vulkan/vulkan.h>

#include <vk_mem_alloc.h>

struct AllocatedBuffer {
	VkBuffer _buffer;
	VmaAllocation _allocation;
};


#endif //BEST_ENGINE_VK_TYPES_H
