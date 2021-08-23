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

struct Texture
{
	AllocatedImage image;
	VkImageView imageView;
};

struct BufferBuilder
{
public:
	AllocatedBuffer build();

	BufferBuilder& withSize(size_t size) { allocSize = size; return *this; }
	BufferBuilder& withDebugName(std::string name) { debugName = name; return *this; }
	BufferBuilder& withAllocator(VmaAllocator all) { _allocator = all; return *this; }

	BufferBuilder& withUsageForUniform() { usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | usage.value_or(0); return *this; }
	BufferBuilder& withUsageForStorage() { usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | usage.value_or(0); return *this; }
	BufferBuilder& withUsageForTransfer() { usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT | usage.value_or(0); return *this; }
	BufferBuilder& withUsageForTransferSource() { usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT | usage.value_or(0); return *this; }
	BufferBuilder& withUsageForVertex() { usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | usage.value_or(0); return *this; }

	BufferBuilder& withMemoryCPUOnly() { assert(!memoryUsage.has_value()); memoryUsage = VmaMemoryUsage::VMA_MEMORY_USAGE_CPU_ONLY; return *this; }
	BufferBuilder& withMemoryGPUOnly() { assert(!memoryUsage.has_value()); memoryUsage = VmaMemoryUsage::VMA_MEMORY_USAGE_GPU_ONLY; return *this; }
	BufferBuilder& withMemoryCPUGPU() { assert(!memoryUsage.has_value()); memoryUsage = VmaMemoryUsage::VMA_MEMORY_USAGE_CPU_TO_GPU; return *this; }
	

private:
	std::optional <size_t> allocSize;
	std::optional <VkBufferUsageFlags> usage;
	std::optional <VmaMemoryUsage> memoryUsage;
	std::optional <std::string> debugName;
	std::optional <VmaAllocator> _allocator;
};