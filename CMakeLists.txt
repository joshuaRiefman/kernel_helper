cmake_minimum_required(VERSION 3.24)
project(kernels)

set(CMAKE_CXX_STANDARD 23)

add_executable(kernel_helper src/kernel_helper.c)