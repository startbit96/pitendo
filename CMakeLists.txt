cmake_minimum_required(VERSION 3.13)

project(pitendo)

include_directories(    ${PROJECT_SOURCE_DIR}/inc/
                        ${PROJECT_SOURCE_DIR}/inc/pitendo_game_engine
                        ${PROJECT_SOURCE_DIR}/inc/template
                        ${PROJECT_SOURCE_DIR}/inc/flappy_bird)

add_executable(pitendo  pitendo.cpp 
                        src/pitendo_game_engine/controller.cpp 
                        src/pitendo_game_engine/display.cpp 
                        src/pitendo_game_engine/pitendo_game_engine.cpp
                        src/template/template.cpp
                        src/flappy_bird/flappy_bird.cpp)

LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/res)
target_link_libraries (pitendo wiringPi)