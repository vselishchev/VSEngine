cmake_minimum_required(VERSION 3.1)

set(SOURCE_DIR "${ROOT_DIR}/Code")

include_directories(${SOURCE_DIR})

set(SRC_CORE
	"Core/Engine.h"
	"Core/Engine.cpp")

set(SRC_OBJECT_MODEL 	
	"ObjectModel/Material.h"
	"ObjectModel/Material.cpp"
	"ObjectModel/Mesh.h"
	"ObjectModel/Mesh.cpp"
	"ObjectModel/MeshCollection.h"
	"ObjectModel/MeshCollection.cpp"
	"ObjectModel/Vertex.h"
	"ObjectModel/Vertex.cpp")

set(SRC_RENDERER
	"Renderer/RenderData.h"
	"Renderer/RenderData.cpp"
	"Renderer/Renderer.h"
	"Renderer/Renderer.cpp"
	"Renderer/Shader.h"
	"Renderer/Shader.cpp"
	"Renderer/ShaderProgram.h"
	"Renderer/ShaderProgram.cpp")

set(SRC_SCENE
	"Scene/Scene.h"
	"Scene/Scene.cpp"
	"Scene/Utils.h"
	"Scene/Utils.cpp")

set(SRC_SCENE_COMPONENTS
	"Scene/Components/Camera.h"
	"Scene/Components/Camera.cpp"
	"Scene/Components/Light.h"
	"Scene/Components/Light.cpp"
	"Scene/Components/SceneObject.h"
	"Scene/Components/SceneObject.cpp")

set(SRC_SHADERS
	"Shaders/fragment.fs.glsl"
	"Shaders/vertex.vs.glsl"
	"Shaders/Light.fs.glsl"
	"Shaders/Light.vs.glsl")

set(SRC_SPATIAL_SYSTEM
	"SpatialSystem/Octree.h"
	"SpatialSystem/Octree.cpp")

set(SRC_UTILS
	"Utils/Color.h"
	"Utils/Color.cpp"
	"Utils/CommonUtils.h"
	"Utils/CommonUtils.cpp"
	"Utils/GeometryUtils.cpp"
	"Utils/GeometryUtils.h")

source_group("Core"              REGULAR_EXPRESSION "Core/.*")
source_group("ObjectModel"       REGULAR_EXPRESSION "ObjectModel/.*")
source_group("Renderer"          REGULAR_EXPRESSION "Renderer/.*")
source_group("Scene"             REGULAR_EXPRESSION "Scene/.*")
source_group("Scene\\Components" REGULAR_EXPRESSION "Scene/Components/.*")
source_group("Shaders"           REGULAR_EXPRESSION "Shaders/.*")
source_group("SpatialSystem"     REGULAR_EXPRESSION "SpatialSystem/.*")
source_group("Utils"             REGULAR_EXPRESSION "Utils/.*")

set(SRC "${SRC_CORE}" 
		"${SRC_OBJECT_MODEL}"
		"${SRC_RENDERER}"
		"${SRC_SCENE}"
		"${SRC_SCENE_COMPONENTS}"
		"${SRC_SHADERS}"
		"${SRC_SPATIAL_SYSTEM}"
		"${SRC_UTILS}")

add_executable(${PROJECT_NAME} 
	"Entry.cpp"
	"${SRC}")

add_compile_definitions(ROOT_PATH="${ROOT_DIR}")
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}")
