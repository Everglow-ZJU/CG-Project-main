cmake_minimum_required(VERSION 3.10)

project(final_project)

set(THIRD_PARTY_LIBRARY_PATH ${CMAKE_SOURCE_DIR}/external)

include_directories(${THIRD_PARTY_LIBRARY_PATH}/glm)

include_directories(${THIRD_PARTY_LIBRARY_PATH}/glad/include)

include_directories(${THIRD_PARTY_LIBRARY_PATH}/glfw/include)

include_directories(${THIRD_PARTY_LIBRARY_PATH}/tiny_obj_loader)

include_directories(${THIRD_PARTY_LIBRARY_PATH}/imgui)

include_directories(${THIRD_PARTY_LIBRARY_PATH}/stb)

aux_source_directory(. PROJECT_SRC)

set(BASE_SRC ../base/application.cpp
        ../base/shader.cpp
        ../base/camera.cpp
        ../base/object3d.cpp
        ../base/model.cpp
        ../base/skybox.cpp
        ../base/texture.cpp
        ../final_project/helicopter.cpp
        ../final_project/house.cpp
        ../final_project/door.cpp
        ../final_project/lamp.cpp
        ../final_project/AABBBox.cpp
        ../final_project/screenShot.cpp
        ../final_project/obj_seq.cpp
        ../final_project/HouseBox.cpp
        ../final_project/ballModel.cpp
        ../final_project/cylinder.cpp)

add_executable(final_project ${PROJECT_SRC} ${BASE_SRC})

if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
elseif(UNIX)
    if (CMAKE_BUILD_TYPE MATCHES Debug)
        set_target_properties(${PROJECT_NAME} PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Debug")
    else()
        set_target_properties(${PROJECT_NAME} PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/Release")
    endif()
endif()

target_link_libraries(final_project glm)

target_link_libraries(final_project glad)

target_link_libraries(final_project glfw)

target_link_libraries(final_project tiny_obj_loader)

target_link_libraries(final_project imgui)

target_link_libraries(final_project stb)