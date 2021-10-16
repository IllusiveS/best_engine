#include "DescriptorLayoutCache.h"

#include <algorithm>

void DescriptorLayoutCache::init(VkDevice newDevice)
{
	device = newDevice;
}
void DescriptorLayoutCache::cleanup()
{
	//delete every descriptor layout held
	for(auto pair : layoutCacheL1)
	{
		vkDestroyDescriptorSetLayout(device, pair.second, nullptr);
	}
}

VkDescriptorSetLayout DescriptorLayoutCache::create_descriptor_layout(VkDescriptorSetLayoutCreateInfo* info)
{
	DescriptorLayoutInfo layoutinfo;
	layoutinfo.info = info;
	layoutinfo.bindings.reserve(info->bindingCount);
	bool isSorted = true;
	int lastBinding = -1;

	//copy from the direct info struct into our own one
	for(int i = 0; i < info->bindingCount; i++)
	{
		layoutinfo.bindings.push_back(info->pBindings[i]);

		//check that the bindings are in strict increasing order
		if(info->pBindings[i].binding > lastBinding)
		{
			lastBinding = info->pBindings[i].binding;
		}
		else
		{
			isSorted = false;
		}
	}
	//sort the bindings if they aren't in order
	if(!isSorted)
	{
		std::sort(layoutinfo.bindings.begin(), layoutinfo.bindings.end(), [](VkDescriptorSetLayoutBinding& a, VkDescriptorSetLayoutBinding& b)
			{
				return a.binding < b.binding;
			});
	}



	//try to grab from cache L1
	auto it = create_descriptor_layout_L1(layoutinfo);
	if(it)
	{
		return *it;
	}
	else
	{
		return create_descriptor_layout_L2(layoutinfo);
	}
}

std::optional<VkDescriptorSetLayout> DescriptorLayoutCache::create_descriptor_layout_L1(const DescriptorLayoutInfo& info)
{
	auto it = layoutCacheL1.find(info);
	if(it != layoutCacheL1.end())
	{
		return (*it).second;
	}
	return std::nullopt;
}

VkDescriptorSetLayout DescriptorLayoutCache::create_descriptor_layout_L2(const DescriptorLayoutInfo& info)
{
	std::lock_guard lock(L2cacheLock);
	auto it = layoutCacheL2.find(info);
	if(it != layoutCacheL2.end())
	{
		return (*it).second;
	}
	else
	{
		//create a new one (not found) on cache L2
		VkDescriptorSetLayout layout;
		vkCreateDescriptorSetLayout(device, info.info, nullptr, &layout);

		//add to cache
		layoutCacheL1[info] = layout;
		return layout;
	}
}

void DescriptorLayoutCache::update_L1_cache()
{
	std::lock_guard lock(L2cacheLock);
	for( auto& layout : layoutCacheL2)
	{
		if(layoutCacheL1.find(layout.first) != std::end(layoutCacheL1))
			continue;
		layoutCacheL1[layout.first] = layout.second;
	}
	layoutCacheL2.clear();
}

bool DescriptorLayoutCache::DescriptorLayoutInfo::operator==(const DescriptorLayoutInfo& other) const
{
	if(other.bindings.size() != bindings.size())
	{
		return false;
	}
	else
	{
		//compare each of the bindings is the same. Bindings are sorted so they will match
		for(int i = 0; i < bindings.size(); i++)
		{
			if(other.bindings[i].binding != bindings[i].binding)
			{
				return false;
			}
			if(other.bindings[i].descriptorType != bindings[i].descriptorType)
			{
				return false;
			}
			if(other.bindings[i].descriptorCount != bindings[i].descriptorCount)
			{
				return false;
			}
			if(other.bindings[i].stageFlags != bindings[i].stageFlags)
			{
				return false;
			}
		}
		return true;
	}
}

size_t DescriptorLayoutCache::DescriptorLayoutInfo::hash() const
{
	using std::size_t;
	using std::hash;

	size_t result = hash<size_t>()(bindings.size());

	for(const VkDescriptorSetLayoutBinding& b : bindings)
	{
		//pack the binding data into a single int64. Not fully correct but it's ok
		size_t binding_hash = b.binding | b.descriptorType << 8 | b.descriptorCount << 16 | b.stageFlags << 24;

		//shuffle the packed binding data and xor it with the main hash
		result ^= hash<size_t>()(binding_hash);
	}

	return result;
}
