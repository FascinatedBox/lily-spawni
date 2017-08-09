cmake_minimum_required(VERSION 3.0.0)
project("lily-spawni")

include_directories("${CMAKE_INSTALL_PREFIX}/include/lily/")

add_library(spawni SHARED src/lily_spawni.c)
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/src")

set_target_properties(spawni PROPERTIES PREFIX "")
