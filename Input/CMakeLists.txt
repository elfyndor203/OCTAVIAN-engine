add_library(Input STATIC
    system/systemDescription.c
        internal/inputSystem_int.h
        internal/types_int.h
        internal/inputSystem.c
        internal/input/inputs_int.h
        internal/input/callbacks.c
        system/systemDescription_input.h
        internal/input/inputs.c
        include/OCT_Inputs.h
        include/inputs.h
        internal/input/types_int.h
        internal/interact/interactBox2D_int.h
        internal/interact/types_int.h
        internal/interact/interactBox2D.c
        include/interactBox2D.h
)

target_include_directories(Input
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/internal
        ${CMAKE_CURRENT_SOURCE_DIR}/system
)

set_target_properties(Input PROPERTIES
        C_STANDARD 11
        C_STANDARD_REQUIRED ON
        C_EXTENSIONS OFF
)

add_library(InputSystem INTERFACE)
target_include_directories(InputSystem INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/system)

target_link_libraries(Input PUBLIC Core)