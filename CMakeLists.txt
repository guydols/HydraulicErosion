cmake_minimum_required(VERSION 3.15)
project(HydraulicErosion)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Ofast -Wall -Wextra")

add_executable(HydraulicErosion main.cpp HydraulicErosion.cpp Tool.cpp)