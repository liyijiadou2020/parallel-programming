cmake_minimum_required(VERSION 3.26)
project(04_paralell_computing)

set(CMAKE_CXX_STANDARD 11)

add_executable(04_paralell_computing main.cpp)

# openMP 配置
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")