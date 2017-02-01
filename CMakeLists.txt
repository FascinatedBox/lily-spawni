cmake_minimum_required(VERSION 3.0.0)
project("lily-spawni")

include_directories("${CMAKE_INSTALL_PREFIX}/include/lily/")

add_library(spawni SHARED lily_spawni.c)
set_target_properties(spawni PROPERTIES PREFIX "")
