
add_library(_SYSTEM_TEST STATIC
    internal/systemDescription.c
    internal/system/systemEx.c
    internal/system/systems_succeed.c
)

target_include_directories(_SYSTEM_TEST
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/internal
)

set_target_properties(_SYSTEM_TEST PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    C_EXTENSIONS OFF
)

target_link_libraries(_SYSTEM_TEST PUBLIC Core)