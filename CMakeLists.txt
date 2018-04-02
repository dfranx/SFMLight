cmake_minimum_required(VERSION 2.6)

set(EXECUTABLE_NAME "SFMLight")

project(${EXECUTABLE_NAME})

if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX -d)
endif()

include_directories(inc)

add_library(${EXECUTABLE_NAME} SHARED
	src/Light.cpp
	src/LightScene.cpp
	src/Line.cpp
	src/Object.cpp
)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

find_package(SFML 2 REQUIRED graphics window system)
find_package(Thor 2 REQUIRED)

if(THOR_FOUND)
	include_directories(${THOR_INCLUDE_DIR})
	include_directories(${THOR_ROOT}/extlibs/aurora/include)
	target_link_libraries(${EXECUTABLE_NAME} ${THOR_LIBRARY})
endif()

if(SFML_FOUND)
  include_directories(${SFML_INCLUDE_DIR})
  target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
endif()

add_subdirectory(examples)