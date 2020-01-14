cmake_minimum_required(VERSION 3.13)

project(VSEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /O2 /Wall")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
endif()

#set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(OUTPUT_DIR "${ROOT_DIR}/bin/Windows")

# Includes
set(THIRDPARTY_DIR "${ROOT_DIR}/3rdParty")
include_directories("${THIRDPARTY_DIR}" 
					"${THIRDPARTY_DIR}/glm"
					"${THIRDPARTY_DIR}/assimp/include"
					"${THIRDPARTY_DIR}/glew-2.1.0/include"
					"${THIRDPARTY_DIR}/glfw-3.2.1/include/GLFW"
					)

add_subdirectory(Code)

set(LIBRARIES_DIR "${OUTPUT_DIR}/Debug")

find_library(IrrXML_LIB "IrrXML" PATHS ${LIBRARIES_DIR})
if (IrrXML_LIB)
	target_link_libraries(${PROJECT_NAME} ${IrrXML_LIB})
elseif()
	message("IrrXML static library not found!")
endif()

find_library(ZLIB_LIB "zlibstatic" PATHS ${LIBRARIES_DIR})
if (ZLIB_LIB)
	target_link_libraries(${PROJECT_NAME} ${ZLIB_LIB})
elseif()
	message("ZLIB static library not found!")
endif()

find_library(ASSIMP_LIB "assimp" PATHS ${LIBRARIES_DIR})
if (ASSIMP_LIB)
	target_include_directories(${PROJECT_NAME} PUBLIC "${THIRDPARTY_DIR}/assimp/include")
	target_link_libraries(${PROJECT_NAME} ${ASSIMP_LIB})
elseif()
	message("Assimp static library not found!")
endif()

find_library(GLEW32_LIB "glew32" PATHS ${LIBRARIES_DIR})
if (GLEW32_LIB)
	target_include_directories(${PROJECT_NAME} PUBLIC "${THIRDPARTY_DIR}/glew-2.1.0/include")
	target_link_libraries(${PROJECT_NAME} ${GLEW32_LIB})
elseif()
	message("GLEW32 static library not found!")
elseif()

endif()

find_library(GLEW32s_LIB "glew32s" PATHS ${LIBRARIES_DIR})
if (GLEW32s_LIB)
	target_link_libraries(${PROJECT_NAME} ${GLEW32s_LIB})
elseif()
	message("GLEW32s static library not found!")
endif()

find_library(GLFW3_LIB "glfw3" PATHS ${LIBRARIES_DIR})
if (GLFW3_LIB)
	target_include_directories(${PROJECT_NAME} PUBLIC "${THIRDPARTY_DIR}/glfw3-3.2.1/include/glfw")
	target_link_libraries(${PROJECT_NAME} ${GLFW3_LIB})
elseif()
	message("GLFW3 static library not found!")
endif()

find_library(OGL32_LIB "OpenGL32" PATHS ${LIBRARIES_DIR})
if (OGL32_LIB)
	target_link_libraries(${PROJECT_NAME} ${OGL32_LIB})
elseif()
	message("OpenGL32 static library not found!")
endif()