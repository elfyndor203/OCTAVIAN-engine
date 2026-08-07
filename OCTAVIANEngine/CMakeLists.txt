add_library(OCTAVIANEngine STATIC
        internal/engine/engine.c
        SYSTEMS_BUILD/REGISTRATIONS.c
        SYSTEMS_BUILD/scheduler.c
        internal/system/engineSystem.h
        internal/system/types_int.h
        SYSTEMS_BUILD/HEADERS.h
)

target_include_directories(OCTAVIANEngine
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
            ${CMAKE_CURRENT_SOURCE_DIR}/SYSTEMS_BUILD
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/internal
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
        Renderer
        Input
        Platform
        Physics
    PRIVATE
        WorldSystem
        _SystemExSystem
        CoreInit
        RendererSystem
        InputSystem
        PlatformSystem
        PhysicsSystem
)