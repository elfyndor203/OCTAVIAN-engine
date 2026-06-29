include(FetchContent)
FetchContent_Declare(glfw
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.4 #__NOTE__ use hash?
)
FetchContent_MakeAvailable(glfw)

add_library(glad STATIC ${CMAKE_CURRENT_SOURCE_DIR}/GLAD/src/glad.c )

target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/GLAD/include)