cmake_minimum_required(VERSION 3.10)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(cg_project VERSION 1.0)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

file(COPY "media/" DESTINATION "media")

add_subdirectory(./external/glad)

add_subdirectory(./external/glm)

add_subdirectory(./external/glfw)

add_subdirectory(./external/tiny_obj_loader)

add_subdirectory(./external/imgui)

add_subdirectory(./external/stb)

add_subdirectory(./projects/final_project)
