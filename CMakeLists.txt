cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

set(PROJECT_NAME ecs_root)
project(${PROJECT_NAME})

option(COMPILE_64_BIT "Compile as 64 bit?" ON)
if (${COMPILE_64_BIT})
    set(CMAKE_CXX_FLAGS -m64)
    set(CMAKE_LXX_FLAGS -m64)
else()
    set(CMAKE_CXX_FLAGS -m32)
    set(CMAKE_LXX_FLAGS -m32)
endif()

add_definitions(-Wall -Wconversion -Wextra -Werror -pedantic -std=c++11)

add_subdirectory(ecs)
add_subdirectory(tests)
