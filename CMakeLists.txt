cmake_minimum_required(VERSION 3.24)
project(kernel_helper)

set(CMAKE_CXX_STANDARD 23)

add_library(kernel_helper STATIC src/kernel_helper.c)
set_target_properties(kernel_helper PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

add_executable(convert_kernel src/convert_kernel.c include/convert_kernel.h src/kernel_helper.c include/kernel_helper.h)
set_target_properties(convert_kernel PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")