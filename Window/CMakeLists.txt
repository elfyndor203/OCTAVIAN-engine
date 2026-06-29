
add_library(Window STATIC
        system/systemDescription.c
        internal/window/window.c
        internal/windowSystem/windowSystem.c
)

target_include_directories(Window
        PUBLIC
            ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/internal
        ../External/GLAD/include
            ${CMAKE_CURRENT_SOURCE_DIR}/GLFW/include
)

set_target_properties(Window PROPERTIES
        C_STANDARD 11
        C_STANDARD_REQUIRED ON
        C_EXTENSIONS OFF
)

add_library(WindowSystem INTERFACE)
target_include_directories(WindowSystem INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/system)

target_link_libraries(Window PUBLIC Core PRIVATE glfw glad)