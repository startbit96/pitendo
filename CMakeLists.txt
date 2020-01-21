cmake_minimum_required(VERSION 3.13)

project(pintendo)

include_directories(${PROJECT_SOURCE_DIR}/inc/)

add_executable(pintendo main.cpp src/controller.cpp src/display.cpp)

LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/res)
target_link_libraries (pintendo wiringPi)