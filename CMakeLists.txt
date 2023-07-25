cmake_minimum_required(VERSION 3.24)
project(kernel_helper C)

add_library(kernel_helper STATIC src/kernel_helper.c)
set_target_properties(kernel_helper PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

add_executable(convert_kernel src/convert_kernel.c include/convert_kernel.h src/kernel_helper.c include/kernel_helper.h)
set_target_properties(convert_kernel PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

install(TARGETS kernel_helper EXPORT KernelHelperConfig ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
export(TARGETS kernel_helper NAMESPACE kernel_helper:: FILE "${CMAKE_CURRENT_BINARY_DIR}/KernelHelperConfig.cmake")
install(EXPORT KernelHelperConfig DESTINATION "${CMAKE_INSTALL_DATADIR}/KernelHelper/cmake" NAMESPACE kernel_helper::)