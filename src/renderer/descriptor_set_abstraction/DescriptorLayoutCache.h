#pragma once

#include <vulkan/vulkan.h>

#include <unordered_map>

#include <mutex>
#include <optional>

class DescriptorLayoutCache
{
public:
	void init(VkDevice newDevice);
	void cleanup();

	VkDescriptorSetLayout create_descriptor_layout(VkDescriptorSetLayoutCreateInfo* info);

	void update_L1_cache();

	struct DescriptorLayoutInfo
	{
		VkDescriptorSetLayoutCreateInfo* info;
		//good idea to turn this into a inlined array
		std::vector<VkDescriptorSetLayoutBinding> bindings;

		bool operator==(const DescriptorLayoutInfo& other) const;

		size_t hash() const;
	};



private:

	std::optional<VkDescriptorSetLayout> create_descriptor_layout_L1(const DescriptorLayoutInfo& info);
	VkDescriptorSetLayout create_descriptor_layout_L2(const DescriptorLayoutInfo& info);

	struct DescriptorLayoutHash
	{

		std::size_t operator()(const DescriptorLayoutInfo& k) const
		{
			return k.hash();
		}
	};

	std::unordered_map<DescriptorLayoutInfo, VkDescriptorSetLayout, DescriptorLayoutHash> layoutCacheL1;
	std::unordered_map<DescriptorLayoutInfo, VkDescriptorSetLayout, DescriptorLayoutHash> layoutCacheL2;

	VkDevice device;


	std::mutex L2cacheLock;
};
