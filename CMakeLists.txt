cmake_minimum_required(VERSION 3.1)
project(VSEngine)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O3 -Wall -Wno-reorder -Wno-sign-compare -Wno-strict-aliasing")

file(GLOB SRC_GEOM ./Geometry/*.cpp ./Geometry/*.h)
file(GLOB SRC_UTILS ./Utils/*.cpp ./Utils/*.h)

set(SRC ${SRC_UTILS} ${SRC_GEOM})

include_directories(./include/GLEW)
include_directories(./include/GLFW)
link_directories(./lib)

add_executable(${PROJECT_NAME} ${SRC} "main.cpp")
target_link_libraries(${PROJECT_NAME} GLEW glfw3)
