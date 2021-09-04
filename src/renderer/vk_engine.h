//
// Created by patwys on 5/5/2021.
//

#ifndef BEST_ENGINE_VK_ENGINE_H
#define BEST_ENGINE_VK_ENGINE_H

#include "vk_types.h"

#include "vk_buffer.h"

#include <vector>
#include <functional>
#include <deque>
#include <unordered_map>
#include <thread>

#include "vk_mesh.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <entt.hpp>

#include "taskflow.hpp"

#include <flecs.h>

#include <TracyVulkan.hpp>


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

	VkCommandBuffer _tracyBuffer;

	AllocatedBuffer objectBuffer;
	VkDescriptorSet objectDescriptor;

	AllocatedBuffer cameraBuffer;

	VkDescriptorSet globalDescriptor;

	std::map<std::thread::id, int> threads;
	std::vector< VkCommandPool > commandPools;
	std::vector< VkCommandBuffer > commandBuffers;
};


struct Material {
	VkPipeline pipeline;
	VkPipelineLayout pipelineLayout;
	Texture* albedo;
};

struct RenderObject {
	Mesh* mesh;

	Material* material;
};

struct MeshPushConstants {
	glm::vec4 data;
	glm::mat4 render_matrix;
};

struct DebugPushConstants
{
	glm::vec4 points[2];
	glm::vec4 color;
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

struct UploadContext
{
	VkFence _uploadFence;
	VkCommandPool _commandPool;
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
	VulkanEngine();

	static PFN_vkSetDebugUtilsObjectNameEXT setObjectDebugName;

	UploadContext _uploadContext;
	void immediate_submit(std::function<void(VkCommandBuffer cmd)>&& function);

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
	VkPipelineLayout _debugPipelineLayout;

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
	std::unordered_map<std::string, Mesh> _meshes;
	std::unordered_map<std::string, Texture> _loadedTextures;

	void load_image(const std::string& path, const std::string& texName);
	void load_images();

	VkDescriptorSetLayout _globalSetLayout;
	VkDescriptorSetLayout _objectSetLayout;
	VkDescriptorSetLayout _singleTextureSetLayout;

	VkDescriptorPool _descriptorPool;

	VkPhysicalDeviceProperties _gpuProperties;

	GPUSceneData _sceneParameters;
	AllocatedBuffer _sceneParameterBuffer;

	//tf::Executor executor{ 2 };
	//tf::Taskflow taskflow;

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

#if defined(NDEBUG)
	const bool debug = false;
#else
	const bool debug = true;
#endif

	constexpr bool isProfilingEnabled() { return debug; };
	constexpr bool areValidationLayersEnabled() { return debug; };

private:
	void init_vulkan();
	void init_swapchain(); 
	void init_commands();
	void init_default_renderpass();
	void init_framebuffers();
	void init_sync_structures();
	void init_pipelines();
	void init_tracy();

	void init_textured_pipeline();
	void init_debug_pipeline();

	void init_imgui();

	void load_meshes();
	void init_scene(flecs::world& world);

	void upload_mesh(Mesh& mesh);

	tracy::VkCtx* _tracyContext;

	size_t pad_uniform_buffer_size(size_t originalSize);
	
	int worldThreads{ 3 };
	tf::Executor executor;
};

VKAPI_ATTR VkBool32 VKAPI_CALL debug_utils_messenger_callback(
		VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
		VkDebugUtilsMessageTypeFlagsEXT message_type,
		const VkDebugUtilsMessengerCallbackDataEXT* callback_data,
		void* user_data);


#endif //BEST_ENGINE_VK_ENGINE_H
