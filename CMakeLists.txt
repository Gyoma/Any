cmake_minimum_required(VERSION 3.15)

project(Any VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDART 14)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(PROJECT_SOURCES 
	src/Any.h
	src/Any.cpp
)

add_library(${PROJECT_NAME} STATIC
	${PROJECT_SOURCES}
)

target_include_directories(${PROJECT_NAME} 
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)
