cmake_minimum_required(VERSION 3.13)

project(pitendo)

include_directories(${PROJECT_SOURCE_DIR}/inc/)

add_executable(pitendo  pitendo.cpp 
                        src/controller.cpp 
                        src/display.cpp 
                        src/pitendo_game_engine.cpp)

LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/res)
target_link_libraries (pitendo wiringPi)