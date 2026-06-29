add_library(Renderer STATIC
    internal/renderer/spriteData/spriteData.c # __NOTE__
)

target_include_directories(Renderer
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/internal
)

set_target_properties(Renderer PROPERTIES
        C_STANDARD 11
        C_STANDARD_REQUIRED ON
        C_EXTENSIONS OFF
)

target_link_libraries(Renderer PUBLIC Core PRIVATE glad)

add_library(RendererSystem INTERFACE)
target_include_directories(RendererSystem INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/system)