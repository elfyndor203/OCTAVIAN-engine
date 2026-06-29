
cmake_minimum_required(VERSION 3.15)
project(OCT_Engine C)

add_subdirectory(External)

add_subdirectory(Core)
add_subdirectory(_SystemEx)
add_subdirectory(World)
add_subdirectory(Window)
add_subdirectory(Renderer)
add_subdirectory(OCTAVIANEngine)

add_subdirectory(GAME)
