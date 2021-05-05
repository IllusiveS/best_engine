cmake_minimum_required(VERSION 3.19)
project(best_engine)

set(CMAKE_CXX_STANDARD 20)

include_directories(third_party/tracy/)

add_executable(best_engine main.cpp)

target_compile_definitions(dunno PRIVATE TRACY_ENABLE)

find_package(Vulkan REQUIRED)

#SDL2.0
set(SDL2_DIR "P:/zig/sdl2.0")
set(SDL2_INCLUDE_DIR "${SDL2_DIR}/include")

# Support both 32 and 64 bit builds
if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
    set(SDL2_LIBRARIES "${SDL2_DIR}/lib/x64/SDL2.lib;${SDL2_DIR}/lib/x64/SDL2main.lib")
else ()
    set(SDL2_LIBRARIES "${SDL2_DIR}/lib/x86/SDL2.lib;${SDL2_DIR}/lib/x86/SDL2main.lib")
endif ()

string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)


include_directories(third_party/glm/)
include_directories(third_party/entt/)
include_directories(third_party/)
include_directories(third_party/wren/)
include_directories(${SDL2_INCLUDE_DIR})

IF (WIN32)
    target_compile_definitions(dunno PRIVATE VK_USE_PLATFORM_WIN32_KHR)
ELSE()
    target_compile_definitions(dunno PRIVATE VK_USE_PLATFORM_XLIB_KHR)
ENDIF()

target_include_directories(dunno PRIVATE Vulkan::Vulkan)
target_include_directories(dunno PRIVATE ${SDL2_INCLUDE_DIR})

target_link_libraries(dunno Vulkan::Vulkan)
target_link_libraries(dunno ${SDL2_LIBRARIES})