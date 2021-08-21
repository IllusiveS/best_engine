//
// Created by patwys on 5/5/2021.
//

#ifndef BEST_ENGINE_VK_ENGINE_H
#define BEST_ENGINE_VK_ENGINE_H

#include "vk_types.h"

#include <vector>
#include <functional>
#include <deque>
#include <unordered_map>

#include "vk_mesh.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <entt.hpp>

#include <flecs.h>

constexpr unsigned int FRAME_OVERLAP = 2;

struct GPUObjectData{
	glm::mat4 modelMatrix;
};

struct GPUSceneData {
	glm::vec4 fogColor; // w is for exponent
	glm::vec4 fogDistances; //x for min, y for max, zw unused.
	glm::vec4 ambientColor;
	glm::vec4 sunlightDirection; //w for sun power
	glm::vec4 sunlightColor;
};

struct GPUCameraData{
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 viewproj;
};

struct FrameData {
	VkSemaphore _presentSemaphore, _renderSemaphore;
	VkFence _renderFence;

	VkCommandPool _commandPool;
	VkCommandBuffer _mainCommandBuffer;

	AllocatedBuffer objectBuffer;
	VkDescriptorSet objectDescriptor;

	AllocatedBuffer cameraBuffer;

	VkDescriptorSet globalDescriptor;
};


struct Material {
	VkPipeline pipeline;
	VkPipelineLayout pipelineLayout;
};

struct RenderObject {
	Mesh* mesh;

	Material* material;
};

struct MeshPushConstants {
	glm::vec4 data;
	glm::mat4 render_matrix;
};

struct DeletionQueue
{
	std::deque<std::function<void()>> deletors;

	void push_function(std::function<void()>&& function) {
		deletors.push_back(function);
	}

	void flush() {
		// reverse iterate the deletion queue to execute all the functions
		for (auto it = deletors.rbegin(); it != deletors.rend(); it++) {
			(*it)(); //call functors
		}

		deletors.clear();
	}
};

class PipelineBuilder {
public:

	std::vector<VkPipelineShaderStageCreateInfo> _shaderStages;
	VkPipelineVertexInputStateCreateInfo _vertexInputInfo;
	VkPipelineInputAssemblyStateCreateInfo _inputAssembly;
	VkViewport _viewport;
	VkRect2D _scissor;
	VkPipelineRasterizationStateCreateInfo _rasterizer;
	VkPipelineColorBlendAttachmentState _colorBlendAttachment;
	VkPipelineMultisampleStateCreateInfo _multisampling;
	VkPipelineLayout _pipelineLayout;
	VkPipelineDepthStencilStateCreateInfo _depthStencil;

	VkPipeline build_pipeline(VkDevice device, VkRenderPass pass);
};


class VulkanEngine {
public:

	bool _isInitialized{ false };
	int _frameNumber {0};

	VkExtent2D _windowExtent{ 1700 , 900 };

	struct SDL_Window* _window{ nullptr };

	//initializes everything in the engine
	void init(flecs::world &world);

	//shuts down the engine
	void cleanup();

	//draw loop
	void draw(flecs::world& world);

	//run main loop
	void run(flecs::world& world);

	bool load_shader_module(const char* filePath, VkShaderModule* outShaderModule);

	VkInstance _instance; // Vulkan library handle
	VkDebugUtilsMessengerEXT _debug_messenger; // Vulkan debug output handle
	VkPhysicalDevice _chosenGPU; // GPU chosen as the default device
	VkDevice _device; // Vulkan device for commands
	VkSurfaceKHR _surface; // Vulkan window surface

	VkSwapchainKHR _swapchain; // from other articles

	// image format expected by the windowing system
	VkFormat _swapchainImageFormat;

	//array of images from the swapchain
	std::vector<VkImage> _swapchainImages;

	//array of image-views from the swapchain
	std::vector<VkImageView> _swapchainImageViews;

	VkQueue _graphicsQueue; //queue we will submit to
	uint32_t _graphicsQueueFamily; //family of that queue

	VkRenderPass _renderPass;

	std::vector<VkFramebuffer> _framebuffers;

	//other code ....
	//frame storage
	FrameData _frames[FRAME_OVERLAP];

	//getter for the frame we are rendering to right now.
	FrameData& get_current_frame();

	VkPipelineLayout _trianglePipelineLayout;
	VkPipelineLayout _meshPipelineLayout;

	VkPipeline _trianglePipeline;
	VkPipeline _redTrianglePipeline;

	DeletionQueue _mainDeletionQueue;

	VmaAllocator _allocator; //vma lib allocator

	VkPipeline _meshPipeline;
	Mesh _triangleMesh;

	Mesh _monkeyMesh;

	VkImageView _depthImageView;
	AllocatedImage _depthImage;

	//the format for the depth image
	VkFormat _depthFormat;

	//default array of renderable objects
	std::vector<RenderObject> _renderables;

	std::unordered_map<std::string,Material> _materials;
	std::unordered_map<std::string,Mesh> _meshes;

	VkDescriptorSetLayout _globalSetLayout;
	VkDescriptorSetLayout _objectSetLayout;

	VkDescriptorPool _descriptorPool;

	VkPhysicalDeviceProperties _gpuProperties;

	GPUSceneData _sceneParameters;
	AllocatedBuffer _sceneParameterBuffer;


	//functions
	//create material and add it to the map
	Material* create_material(VkPipeline pipeline, VkPipelineLayout layout, const std::string& name);

	//returns nullptr if it can't be found
	Material* get_material(const std::string& name);

	//returns nullptr if it can't be found
	Mesh* get_mesh(const std::string& name);

	//our draw function
	void draw_objects(VkCommandBuffer cmd,RenderObject* first, int count, flecs::world& world);

	AllocatedBuffer create_buffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);

	void init_descriptors();

	int _selectedShader{ 0 };

private:
	void init_vulkan();
	void init_swapchain(); 
	void init_commands();
	void init_default_renderpass();
	void init_framebuffers();
	void init_sync_structures();
	void init_pipelines();

	void load_meshes();
	void init_scene(flecs::world& world);

	void upload_mesh(Mesh& mesh);

	size_t pad_uniform_buffer_size(size_t originalSize);
};


#endif //BEST_ENGINE_VK_ENGINE_H
