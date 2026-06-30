add_library(Resources STATIC
    system/systemDescription_Resources.c
)

target_include_directories(Resources
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/internal
)

set_target_properties(Resources PROPERTIES
        C_STANDARD 11
        C_STANDARD_REQUIRED ON
        C_EXTENSIONS OFF
)

add_library(ResourcesSystem INTERFACE)
target_include_directories(ResourcesSystem INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/system)

target_link_libraries(Resources PUBLIC Core)