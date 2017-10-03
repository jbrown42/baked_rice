cmake_minimum_required(VERSION 3.8)
project(baked_rice)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Drone.cpp Drone.h World.cpp World.h Mthread.cpp Mthread.h)

set(CMAKE_CXX_FLAGS "-lpthread")
add_executable(baked_rice ${SOURCE_FILES})
