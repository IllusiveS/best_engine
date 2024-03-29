cmake_minimum_required(VERSION 3.19)
project(best_engine)

set(CMAKE_CXX_STANDARD 20)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")

add_executable(best_engine third_party/tracy/TracyClient.cpp main.cpp third_party/tinyobjloader/tiny_obj_loader.cc src/renderer/vk_engine.cpp src/renderer/vk_engine.h src/renderer/vk_init.cpp src/renderer/vk_init.h src/renderer/vk_types.cpp src/renderer/vk_types.h third_party/vk-bootstrap/src/VkBootstrap.cpp src/renderer/vk_mesh.cpp src/renderer/vk_mesh.h src/script/ScriptingMachine.cpp src/script/ScriptingMachine.h src/script/Script.cpp src/script/Script.h src/components/Camera.cpp src/components/Camera.h src/components/Transform.cpp src/components/Transform.h src/components/MeshComponent.cpp src/components/MeshComponent.h "src/renderer/vk_buffer.h" "src/renderer/vk_buffer.cpp" "src/collision/BoxCollisionComponent.h" src/renderer/vk_texture.cpp src/renderer/vk_texture.h "src/input_manager/input_manager.h" "src/input_manager/input_manager.cpp" "src/renderer/descriptor_set_abstraction/DescriptorBuilder.h" "src/renderer/descriptor_set_abstraction/DescriptorAllocator.h" "src/renderer/descriptor_set_abstraction/DescriptorLayoutCache.h" "src/renderer/descriptor_set_abstraction/DescriptorAllocator.cpp" "src/renderer/descriptor_set_abstraction/DescriptorLayoutCache.cpp" "src/renderer/descriptor_set_abstraction/DescriptorBuilder.cpp" "src/collision/Ray.h" "src/collision/CollisionDetectionSystem.cpp" "src/util/md5.h" "src/util/File.h" "src/util/md5.cpp" "src/util/File.cpp" "src/renderer/shader/ShaderManager.h" "src/renderer/shader/ShaderManager.cpp" "src/util/Taskmaster/Taskmaster.h" "src/util/Taskmaster/TaskBuilder.h" "src/util/Taskmaster/Taskmaster.cpp" "src/util/Taskmaster/TaskBuilder.cpp" "src/renderer/shader/ShaderCompiler.h" "src/renderer/shader/ShaderCompiler.cpp" "src/util/Resource.h")

target_compile_definitions(best_engine PRIVATE TRACY_ENABLE)

find_package(Vulkan REQUIRED)

#SDL2.0
set(SDL2_DIR "${PROJECT_SOURCE_DIR}/libs/sdl")
set(SDL2_INCLUDE_DIR "${SDL2_DIR}/include")
# Support both 32 and 64 bit builds
if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
    set(SDL2_LIBRARIES "${SDL2_DIR}/lib/x64/SDL2.lib;${SDL2_DIR}/lib/x64/SDL2main.lib")
else ()
    set(SDL2_LIBRARIES "${SDL2_DIR}/lib/x86/SDL2.lib;${SDL2_DIR}/lib/x86/SDL2main.lib")
endif ()

string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)

target_include_directories(best_engine PUBLIC sdl/include)

add_subdirectory(libs)
add_subdirectory(third_party)

include_directories(src/)
include_directories(third_party)
include_directories(third_party/stb)
include_directories(third_party/vk-bootstrap/src)
include_directories(third_party/taskflow/taskflow)
include_directories(third_party/tracy/)
include_directories(third_party/glm)
include_directories(third_party/wren/src/include)
include_directories(third_party/VulkanMemoryAllocator/include)
include_directories(third_party/tinyobjloader)
include_directories(third_party/imgui)
include_directories(third_party/flecs/include/flecs)

include_directories(${SDL2_INCLUDE_DIR})

IF (WIN32)
    target_compile_definitions(best_engine PRIVATE VK_USE_PLATFORM_WIN32_KHR)
ELSE()
    target_compile_definitions(best_engine PRIVATE VK_USE_PLATFORM_XLIB_KHR)
ENDIF()

target_include_directories(best_engine PUBLIC glslc)
target_include_directories(best_engine PUBLIC shaderc)
target_include_directories(best_engine PRIVATE Vulkan::Vulkan)
target_include_directories(best_engine PRIVATE ${SDL2_INCLUDE_DIR})

target_link_libraries(best_engine glslc)
target_link_libraries(best_engine shaderc)
target_link_libraries(best_engine wren)
target_link_libraries(best_engine Vulkan::Vulkan)
target_link_libraries(best_engine flecs)
target_link_libraries(best_engine ${SDL2_LIBRARIES})
target_link_libraries(best_engine imgui)
target_link_libraries(best_engine imgui_gizmo)

find_program(GLSL_VALIDATOR glslangValidator HINTS /usr/bin /usr/local/bin $ENV{VULKAN_SDK}/Bin/ $ENV{VULKAN_SDK}/Bin32/)

file(GLOB_RECURSE GLSL_SOURCE_FILES
        "${PROJECT_SOURCE_DIR}/shaders/*.frag"
        "${PROJECT_SOURCE_DIR}/shaders/*.vert"
        "${PROJECT_SOURCE_DIR}/shaders/*.comp"
        )

foreach(GLSL ${GLSL_SOURCE_FILES})
    #message(STATUS "BUILDING SHADER")
    get_filename_component(FILE_NAME ${GLSL} NAME)
    set(SPIRV "${PROJECT_SOURCE_DIR}/shaders/${FILE_NAME}.spv")
    #message(STATUS ${GLSL})
    add_custom_command(
            OUTPUT ${SPIRV}
            COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
            DEPENDS ${GLSL})
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
        Shaders
        DEPENDS ${SPIRV_BINARY_FILES}
)