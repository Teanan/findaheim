cmake_minimum_required(VERSION 3.7)

project(Game)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/Modules)

find_package(anax REQUIRED)
find_package(SFML 2.4 REQUIRED COMPONENTS system window graphics)

include_directories(${SFML_INCLUDE_DIR})
include_directories(${ANAX_INCLUDE_DIR})

include_directories(${CMAKE_CURRENT_LIST_DIR}/deps/chipmunkpp/src)
include_directories(${CMAKE_CURRENT_LIST_DIR}/deps/chipmunkpp/chipmunk/include/chipmunk)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

set(SOURCES
	src/Application.cpp
	src/Game.cpp
	src/main.cpp
	src/MathTools.cpp
	src/Scene.cpp
	src/ScenesManager.cpp
	src/TexturesManager.cpp
	include/Application.h
	include/BaseGame.hpp
	include/boxshape.h
	include/Game.hpp
	include/Keyboard.h
	include/MathTools.h
	include/pool.h
	include/pool.inl
	include/RunGame.hpp
	include/Scene.h
	include/ScenesManager.h
	include/TexturesManager.h
	include/Utils.h
)

set(COMPONENTS_SOURCES
	src/Components/AnimationComponent.cpp
	include/Components/AnimationComponent.hpp
	include/Components/CollisionComponent.hpp
	include/Components/LightComponent.h
	include/Components/PhysicsComponent.h
	include/Components/PlayerComponent.hpp
	include/Components/SpriteComponent.hpp
	include/Components/TransformComponent.hpp
	include/Components/VelocityComponent.hpp
)
source_group("Components" FILES ${COMPONENTS_SOURCES})

set(SYSTEMS_SOURCES
	src/Systems/AnimationSystem.cpp
	src/Systems/CollisionSystem.cpp
	src/Systems/LightingSystem.cpp
	src/Systems/MovementSystem.cpp
	src/Systems/PhysicsSystem.cpp
	src/Systems/PlayerInputSystem.cpp
	src/Systems/SpriteRenderingSystem.cpp
	include/Systems/AnimationSystem.hpp
	include/Systems/CollisionSystem.hpp
	include/Systems/LightingSystem.h
	include/Systems/MovementSystem.hpp
	include/Systems/PhysicsSystem.h
	include/Systems/PlayerInputSystem.hpp
	include/Systems/SpriteRenderingSystem.hpp
)
source_group("Systems" FILES ${SYSTEMS_SOURCES})

set(LIGHTING_SOURCES
	src/lighting/LightDirectionEmission.cpp
	src/lighting/LightEngine.cpp
	src/lighting/LightPointEmission.cpp
	src/quadtree/DynamicQuadtree.cpp
	src/quadtree/Quadtree.cpp
	src/quadtree/QuadtreeNode.cpp
	src/quadtree/QuadtreeOccupant.cpp
	src/quadtree/StaticQuadtree.cpp
	include/lighting/LightDirectionEmission.h
	include/lighting/LightEngine.h
	include/lighting/LightPointEmission.h
	include/lighting/LightShape.h
	include/lighting/NormalsSprite.h
	include/quadtree/DynamicQuadtree.h
	include/quadtree/Quadtree.h
	include/quadtree/QuadtreeNode.h
	include/quadtree/QuadtreeOccupant.h
	include/quadtree/StaticQuadtree.h
)
source_group("Lighting" FILES ${LIGHTING_SOURCES})

add_executable(game ${SOURCES} ${COMPONENTS_SOURCES} ${SYSTEMS_SOURCES} ${LIGHTING_SOURCES})

set(CHIPMUNK_LIBRARIES
	${CMAKE_CURRENT_LIST_DIR}/deps/chipmunkpp/visualcpp/Debug/Chipmunk++-d.lib
	${CMAKE_CURRENT_LIST_DIR}/deps/chipmunkpp/chipmunk/msvc/vc13/chipmunk/Win32/Debug/chipmunk.lib
)

# why u no work ?
set(ANAX_LIBRARIES "C:/Program Files (x86)/ANAX/lib/anax_d.lib")

target_link_libraries(game ${SFML_LIBRARIES})
target_link_libraries(game ${ANAX_LIBRARIES})
target_link_libraries(game ${CHIPMUNK_LIBRARIES})