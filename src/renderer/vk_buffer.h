#pragma once

#include "vk_types.h"

#include <optional>
#include <assert.h>

#include <vk_mem_alloc.h>

struct AllocatedBuffer
{
	VkBuffer _buffer;
	VmaAllocation _allocation;
};

struct AllocatedImage
{
	VkImage _image;
	VmaAllocation _allocation;
};

struct BufferBuilder
{
public:
	AllocatedBuffer build();

	BufferBuilder& withSize(size_t size) { allocSize = size; return *this; }
	BufferBuilder& withDebugName(std::string name) { debugName = name; return *this; }
	BufferBuilder& withAllocator(VmaAllocator all) { _allocator = all; return *this; }
	BufferBuilder& withUsageForUniform() { assert(!usage.has_value()); usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT; return *this; }
	BufferBuilder& withUsageForStorage() { assert(!usage.has_value()); usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT; return *this; }

private:
	std::optional <size_t> allocSize;
	std::optional <VkBufferUsageFlags> usage;
	std::optional <VmaMemoryUsage> memoryUsage;
	std::optional <std::string> debugName;
	std::optional <VmaAllocator> _allocator;
};