add_library(OCTAVIANEngine STATIC
        internal/engine/engine.c
        SYSTEMS_BUILD/BUILD_MANIFEST.c
)

target_include_directories(OCTAVIANEngine
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/internal
            ${CMAKE_CURRENT_SOURCE_DIR}/SYSTEMS_BUILD
)

set_target_properties(OCTAVIANEngine PROPERTIES
        C_STANDARD 11
        C_STANDARD_REQUIRED ON
        C_EXTENSIONS OFF
)

target_link_libraries(OCTAVIANEngine
    PUBLIC
        Core
        World
        _SystemEx
        Window
        Renderer
        Input
    PRIVATE
        WorldSystem
        _SystemExSystem
        CoreInit
        WindowSystem
        RendererSystem
        InputSystem
)