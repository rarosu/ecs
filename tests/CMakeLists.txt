# CMake configuration
cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# Project configuration
set(PROJECT_NAME tests)
project(${PROJECT_NAME})

# Find dependencies
find_package(Threads REQUIRED)
find_package(GTest REQUIRED)
set(EXTERNAL_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/../ecs/include/" ${GTEST_INCLUDE_DIRS})
set(EXTERNAL_LIBRARIES ${GTEST_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

include_directories(${EXTERNAL_INCLUDE_DIRS})

# Setup the executable
set(HEADERS )
set(SOURCES src/tests.cpp)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${EXTERNAL_LIBRARIES})
