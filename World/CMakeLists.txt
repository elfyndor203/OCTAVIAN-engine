
add_library(World STATIC
        internal/world/world.c
        internal/transform2D/transform2D.c
        system/systemDescription.c
)

target_include_directories(World
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/internal
)

set_target_properties(World PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
)

add_library(WorldSystem INTERFACE)
target_include_directories(WorldSystem INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/system)

target_link_libraries(World PUBLIC Core)