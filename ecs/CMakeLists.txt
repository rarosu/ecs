# CMake configuration
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# Project configuration
set(PROJECT_NAME ecs)
project(${PROJECT_NAME})

set(ECS_VERSION_MAJOR 0)
set(ECS_VERSION_MINOR 9)
set(ECS_MAX_COMPONENTS 32)
set(ECS_RESERVED_ENTITY_COUNT 1024)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/include/config.h.in"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/config.h"
)

# Setup the executable
set(HEADERS include/ecs.h include/component.h include/entity.h include/entitymanager.h)
set(SOURCES src/system.cpp src/systemmanager.cpp src/entitymanager.cpp src/component.cpp)

add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})
