cmake_minimum_required(VERSION 3.13)

project(pitendo)

include_directories(    ${PROJECT_SOURCE_DIR}/inc/
                        ${PROJECT_SOURCE_DIR}/inc/pitendoGameEngine
                        ${PROJECT_SOURCE_DIR}/inc/template)

add_executable(pitendo  pitendo.cpp 
                        src/pitendoGameEngine/controller.cpp 
                        src/pitendoGameEngine/display.cpp 
                        src/pitendoGameEngine/pitendo_game_engine.cpp
                        src/template/template.cpp)

LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/res)
target_link_libraries (pitendo wiringPi)