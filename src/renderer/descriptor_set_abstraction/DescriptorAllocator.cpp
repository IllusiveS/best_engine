#include "DescriptorAllocator.h"

#include <cassert>

#include <algorithm>

bool IsMemoryError(VkResult errorResult)
{
	switch(errorResult)
	{
	case VK_ERROR_FRAGMENTED_POOL:
	case VK_ERROR_OUT_OF_POOL_MEMORY:
		return true;
	}
	return false;
}

DescriptorAllocatorHandle::~DescriptorAllocatorHandle()
{
	assert(ownerPool);
	ownerPool->ReturnAllocHandle(this);
}


DescriptorAllocatorHandle::DescriptorAllocatorHandle(DescriptorAllocatorHandle&& other)
{
	assert(ownerPool);

	vkPool = other.vkPool;
	poolIdx = other.poolIdx;
	ownerPool = other.ownerPool;

	other.ownerPool = nullptr;
	other.poolIdx = -1;
	other.vkPool = VkDescriptorPool{};
}

void DescriptorAllocator::init(VkDevice newDevice)
{
	device = newDevice;
}

void DescriptorAllocator::cleanup()
{
	//delete every pool held
	/*for(auto p : freePools)
	{
		vkDestroyDescriptorPool(device, p, nullptr);
	}*/
	for(auto p : usedPools)
	{
		vkDestroyDescriptorPool(device, p, nullptr);
	}
}

VkDescriptorPool createPool(VkDevice device, const DescriptorAllocator::PoolSizes& poolSizes, int count, VkDescriptorPoolCreateFlags flags)
{
	std::vector<VkDescriptorPoolSize> sizes;
	sizes.reserve(poolSizes.sizes.size());
	for(auto sz : poolSizes.sizes)
	{
		sizes.push_back({ sz.first, uint32_t(sz.second * count) });
	}
	VkDescriptorPoolCreateInfo pool_info = {};
	pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	pool_info.flags = flags;
	pool_info.maxSets = count;
	pool_info.poolSizeCount = (uint32_t) sizes.size();
	pool_info.pPoolSizes = sizes.data();

	VkDescriptorPool descriptorPool;
	auto result = vkCreateDescriptorPool(device, &pool_info, nullptr, &descriptorPool);

	assert(result == VK_SUCCESS);

	return descriptorPool;
}

DescriptorAllocator* DescriptorAllocator::Create(const VkDevice& device, int nFrames)
{
	DescriptorAllocator* impl = new DescriptorAllocator();
	impl->_device = device;
	impl->_frameIndex = 0;
	impl->_maxFrames = nFrames;
	return impl;
}

bool DescriptorAllocatorHandle::Allocate(const VkDescriptorSetLayout& layout, VkDescriptorSet& builtSet)
{
	VkDescriptorSetAllocateInfo allocInfo;
	allocInfo.pNext = nullptr;
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = vkPool;
	allocInfo.descriptorSetCount = 1;
	allocInfo.pSetLayouts = &layout;

	VkResult result = vkAllocateDescriptorSets(device, &allocInfo, &builtSet);
	if(result != VK_SUCCESS)
	{
		//we reallocate pools on memory error
		if(IsMemoryError(result))
		{
			//out of space need reallocate			

			ownerPool->ReturnAllocHandle(this);

			DescriptorAllocatorHandle newHandle = ownerPool->GetAllocHandle();

			vkPool = newHandle.vkPool;
			poolIdx = newHandle.poolIdx;

			newHandle.vkPool = VkDescriptorPool{};
			newHandle.poolIdx = -1;
			newHandle.ownerPool = nullptr;
			//could be good idea to avoid infinite loop here
			return Allocate(layout, builtSet);
		}
		else
		{
			//stuff is truly broken
			return false;
		}
	}

	return true;
}

void DescriptorAllocatorHandle::Return()
{
	if(ownerPool)
	{
		ownerPool->ReturnAllocHandle(this);
	}

	vkPool = VkDescriptorPool{ };
	poolIdx = -1;
	ownerPool = nullptr;
}

DescriptorAllocatorHandle DescriptorAllocator::GetAllocHandle()
{
	std::lock_guard guard(_mutex);

	bool foundAllocator = false;

	int poolIndex = _frameIndex;

	VkDescriptorPool poolToReturn;

	//try reuse an allocated pool
	if(availablePools.size() != 0)
	{
		poolToReturn = availablePools.back();
		availablePools.pop_back();
		usedPools.push_back(poolToReturn);
		foundAllocator = true;
	}
	
	//need a new pool
	if(!foundAllocator)
	{
		//static pool has to be free-able
		VkDescriptorPoolCreateFlags flags = 0;
		if(poolIndex == 0)
		{
			flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		}

		VkDescriptorPool newPool = createPool(_device, descriptorSizes, 200, flags);

		poolToReturn = newPool;
		usedPools.push_back(poolToReturn);

		foundAllocator = true;
	}

	assert(foundAllocator);

	DescriptorAllocatorHandle newHandle;
	newHandle.ownerPool = this;
	newHandle.poolIdx = poolIndex;
	newHandle.vkPool = poolToReturn;

	return newHandle;
}

void DescriptorAllocator::ReturnAllocHandle(DescriptorAllocatorHandle* handle)
{
	std::lock_guard guard(_mutex);

	auto usedPool = std::find(std::begin(usedPools), std::end(usedPools), handle->vkPool);
	assert(usedPool != std::end(usedPools)); //did not found used pool?

	auto itr = std::remove(std::begin(usedPools), std::end(usedPools), handle->vkPool);

	availablePools.push_back(handle->vkPool);
}

void DescriptorAllocator::reset_pools()
{
	//reset all used pools and add them to the free pools
	for(auto p : usedPools)
	{
		vkResetDescriptorPool(device, p, 0);
		//freePools.push_back(p);
	}

	//clear the used pools, since we've put them all in the free pools
	usedPools.clear();

	//reset the current pool handle back to null
	currentPool = VK_NULL_HANDLE;
}

