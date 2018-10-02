cmake_minimum_required(VERSION 3.1)
project(VSEngine)

if (UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O3 -Wall")
endif()

file(GLOB SRC_GEOM ./Geometry/*.cpp ./Geometry/*.h)
file(GLOB SRC_UTILS ./Utils/*.cpp ./Utils/*.h)
file(GLOB SRC_RENDERER ./Renderer/*.h ./Renderer/*.cpp)

set(SRC ${SRC_UTILS} ${SRC_GEOM} ${SRC_RENDERER})

include_directories(./include/GLEW)
include_directories(./include/GLFW)
link_directories(./lib)

add_executable(${PROJECT_NAME} ${SRC} "main.cpp")
target_link_libraries(${PROJECT_NAME} GLEW glfw3)
