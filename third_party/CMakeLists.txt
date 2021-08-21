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
