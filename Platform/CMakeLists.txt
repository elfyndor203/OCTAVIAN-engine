add_library(Platform STATIC
        system/systemDescription.c
        system/systemDescription_platform.h
        internal/platformSystem.c
        internal/platformSystem_int.h
)

target_include_directories(Platform
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/internal
        ${CMAKE_CURRENT_SOURCE_DIR}/system
)

set_target_properties(Platform PROPERTIES
        C_STANDARD 11
        C_STANDARD_REQUIRED ON
        C_EXTENSIONS OFF
)

target_link_libraries(Platform PUBLIC Core)

add_library(PlatformSystem INTERFACE)
target_include_directories(PlatformSystem INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/system)