#include "vk_buffer.h"

#include "vk_engine.h"

#include <assert.h>

AllocatedBuffer BufferBuilder::build()
{
	assert(_allocator.has_value());

	//allocate vertex buffer
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.pNext = nullptr;

	bufferInfo.size = allocSize.value_or(0);
	assert(bufferInfo.size > 0);
	bufferInfo.usage = usage.value_or(VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);


	VmaAllocationCreateInfo vmaallocInfo = {};
	vmaallocInfo.usage = memoryUsage.value_or(VMA_MEMORY_USAGE_CPU_TO_GPU);

	AllocatedBuffer newBuffer = {};

	//allocate the buffer
	const auto result = vmaCreateBuffer(_allocator.value(), &bufferInfo, &vmaallocInfo,
		&newBuffer._buffer,
		&newBuffer._allocation,
		nullptr);
	VK_CHECK(result);
	
	if(debugName.has_value())
	{
		VmaAllocatorInfo info;

		vmaGetAllocatorInfo(_allocator.value(), &info);

		auto debugInfo = VkDebugUtilsObjectNameInfoEXT{};
		debugInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
		debugInfo.objectType = VK_OBJECT_TYPE_BUFFER;
		debugInfo.objectHandle = (uint64_t) newBuffer._buffer;
		debugInfo.pObjectName = debugName->c_str();

		VK_CHECK(VulkanEngine::setObjectDebugName(
			info.device
			, &debugInfo)
			);
	}
	
	return newBuffer;
}