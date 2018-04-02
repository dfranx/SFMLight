cmake_minimum_required(VERSION 2.6)

find_package(SFML 2 REQUIRED graphics window system)
find_package(Thor 2 REQUIRED)
find_package(SFMLight REQUIRED)
	
macro(build_example EXAMPLE_NAME)
	project(${EXAMPLE_NAME})

	add_executable(${EXAMPLE_NAME} ${EXAMPLE_NAME}.cpp)
	
	set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

	if(SFML_FOUND)
	  include_directories(${SFML_INCLUDE_DIR})
	  target_link_libraries(${EXAMPLE_NAME} ${SFML_LIBRARIES} ${SFML_DEPENDENCIES})
	endif()
	
	if(THOR_FOUND)
		include_directories(${THOR_INCLUDE_DIR})
		include_directories(${THOR_ROOT}/extlibs/aurora/include)
		target_link_libraries(${EXAMPLE_NAME} ${THOR_LIBRARY})
	endif()
	
	if(SFMLIGHT_FOUND)
		include_directories(${SFMLIGHT_INCLUDE_DIR})
		target_link_libraries(${EXAMPLE_NAME} ${SFMLIGHT_LIBRARY})
	endif()
endmacro()

build_example(test)
build_example(example01)
build_example(example02)
build_example(example03)