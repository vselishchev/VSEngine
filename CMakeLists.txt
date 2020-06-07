cmake_minimum_required(VERSION 3.13)

project(VSEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 /Wall")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
endif()

set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(OUTPUT_DIR "${ROOT_DIR}/bin")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${ROOT_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${ROOT_DIR}/bin)

# Includes
add_subdirectory(Code)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)

conan_basic_setup()
set(THIRDPARTY_DIR "${ROOT_DIR}/3rdParty")
target_include_directories(${PROJECT_NAME} PUBLIC 
							"${THIRDPARTY_DIR}"
							"${CONAN_INCLUDE_DIRS}"
							)

FOREACH(LIBRARY ${CONAN_LIBS})
	find_library(FOUND_LIB "${LIBRARY}" PATHS "${CONAN_LIB_DIRS}")
	if (FOUND_LIB)
		target_link_libraries(${PROJECT_NAME} ${FOUND_LIB})
	elseif()
		message("${FOUND_LIB} static library not found!")
	endif()
	unset(FOUND_LIB CACHE)
ENDFOREACH()

target_link_libraries(${PROJECT_NAME} "OpenGL32")