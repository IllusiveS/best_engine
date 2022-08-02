#WREN
file(GLOB_RECURSE WREN_SOURCE
        "${PROJECT_SOURCE_DIR}/third_party/wren/src/vm/*.c"
        #"${PROJECT_SOURCE_DIR}/third_party/wren/src/vm/*.h"
        "${PROJECT_SOURCE_DIR}/third_party/wren/src/include/*.c"
        #"${PROJECT_SOURCE_DIR}/third_party/wren/src/include/*.h"
        "${PROJECT_SOURCE_DIR}/third_party/wren/src/optional/*.c"
        #"${PROJECT_SOURCE_DIR}/third_party/wren/src/optional/*.h"
        )

add_library(wren STATIC ${WREN_SOURCE} Wrenpp.cpp)

target_include_directories(wren PRIVATE ${PROJECT_SOURCE_DIR}/third_party/wren/src/include)
target_include_directories(wren PRIVATE ${PROJECT_SOURCE_DIR}/third_party/wren/src/optional)
target_include_directories(wren PRIVATE ${PROJECT_SOURCE_DIR}/third_party/wren/src/vm)

add_subdirectory(flecs)

add_compile_definitions(TRACY_ENABLE)

#IMGUI
add_library(imgui STATIC 
    ${PROJECT_SOURCE_DIR}/third_party/imgui/imgui.cpp 
    ${PROJECT_SOURCE_DIR}/third_party/imgui/imgui_draw.cpp 
    ${PROJECT_SOURCE_DIR}/third_party/imgui/imgui_demo.cpp
    ${PROJECT_SOURCE_DIR}/third_party/imgui/imgui_tables.cpp
    ${PROJECT_SOURCE_DIR}/third_party/imgui/imgui_widgets.cpp
    ${PROJECT_SOURCE_DIR}/third_party/imgui/backends/imgui_impl_sdl.cpp
    ${PROJECT_SOURCE_DIR}/third_party/imgui/backends/imgui_impl_vulkan.cpp
    )

target_include_directories(imgui PRIVATE ${Vulkan_INCLUDE_DIRS})
target_include_directories(imgui PRIVATE ${SDL2_INCLUDE_DIR})
target_include_directories(imgui PRIVATE ${PROJECT_SOURCE_DIR}/third_party/imgui)

add_subdirectory(glslang)
add_subdirectory(SPIRV-Tools)
add_subdirectory(shaderc)

#IMGUI GIZMO
add_library(imgui_gizmo STATIC
    ${PROJECT_SOURCE_DIR}/third_party/ImGuizmo/ImGuizmo.cpp
    ${PROJECT_SOURCE_DIR}/third_party/ImGuizmo/ImGuizmo.h
    ${PROJECT_SOURCE_DIR}/third_party/ImGuizmo/ImGradient.cpp
    ${PROJECT_SOURCE_DIR}/third_party/ImGuizmo/ImGradient.h
    ${PROJECT_SOURCE_DIR}/third_party/ImGuizmo/ImCurveEdit.cpp
    ${PROJECT_SOURCE_DIR}/third_party/ImGuizmo/ImCurveEdit.h
    ${PROJECT_SOURCE_DIR}/third_party/ImGuizmo/GraphEditor.cpp
    ${PROJECT_SOURCE_DIR}/third_party/ImGuizmo/GraphEditor.h
    ${PROJECT_SOURCE_DIR}/third_party/ImGuizmo/ImSequencer.cpp
    ${PROJECT_SOURCE_DIR}/third_party/ImGuizmo/ImSequencer.h
    ${PROJECT_SOURCE_DIR}/third_party/ImGuizmo/ImZoomSlider.h
)

target_include_directories(imgui_gizmo PRIVATE ${PROJECT_SOURCE_DIR}/third_party/imgui)

