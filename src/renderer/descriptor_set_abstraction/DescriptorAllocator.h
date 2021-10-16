#pragma once

#include <vulkan/vulkan.h>

#include <vector>

#include <shared_mutex>

class DescriptorLayoutCache;
class DescriptorAllocator;

struct DescriptorAllocatorHandle
{
	friend class DescriptorAllocator;
	DescriptorAllocatorHandle() = default;
	DescriptorAllocatorHandle& operator=(const DescriptorAllocatorHandle&) = delete;

	~DescriptorAllocatorHandle();
	DescriptorAllocatorHandle(DescriptorAllocatorHandle&& other);
	DescriptorAllocatorHandle& operator=(DescriptorAllocatorHandle&& other);

	//return this handle to the pool. Will make this handle orphaned
	void Return();

	//allocate new descriptor. handle has to be valid
	//returns true if allocation succeeded, and false if it didnt
	//will mutate the handle if it requires a new vkDescriptorPool
	bool Allocate(const VkDescriptorSetLayout& layout, VkDescriptorSet& builtSet);


	DescriptorAllocator* ownerPool{ nullptr };
	VkDescriptorPool vkPool;
	int8_t poolIdx;

	VkDevice device;
};

class DescriptorAllocator
{
public:

	struct PoolSizes
	{
		std::vector<std::pair<VkDescriptorType, float>> sizes =
		{
			{ VK_DESCRIPTOR_TYPE_SAMPLER, 0.5f },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 4.f },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 4.f },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1.f },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1.f },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1.f },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2.f },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 2.f },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1.f },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1.f },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 0.5f }
		};
	};

	static DescriptorAllocator* Create(const VkDevice& device, int nFrames = 2);

	//THREAD SAFE
	DescriptorAllocatorHandle GetAllocHandle();
	void ReturnAllocHandle(DescriptorAllocatorHandle* handle);
	//END THREAD SAFE

	void reset_pools();
	bool allocate(VkDescriptorSet* set, VkDescriptorSetLayout layout);

	void init(VkDevice newDevice);

	void cleanup();

	VkDevice device;
private:
	VkDescriptorPool grab_pool();

	VkDescriptorPool currentPool{ VK_NULL_HANDLE };
	PoolSizes descriptorSizes;

	std::vector<VkDescriptorPool> availablePools;
	std::vector<VkDescriptorPool> usedPools;

	VkDevice _device;
	int _frameIndex{ 0 };
	int _maxFrames{ 0 };

	std::mutex _mutex;
};
