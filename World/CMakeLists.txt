
add_library(World STATIC
    internal/system/world.c
    internal/transform2D/transform2D.c
    internal/systemDescription.c
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

target_link_libraries(World PUBLIC Core)