
add_library(_SystemEx STATIC
        system/systemDescription.c
        internal/systemEx/systemEx.c
        internal/systemEx/systems_succeed.c
)

target_include_directories(_SystemEx
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/internal
)

set_target_properties(_SystemEx PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
)

add_library(_SystemExSystem INTERFACE)
target_include_directories(_SystemExSystem INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/system)

target_link_libraries(_SystemEx PUBLIC Core)