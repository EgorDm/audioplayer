cmake_minimum_required(VERSION 3.13)
project(litaudioplayer_project)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(Utils.cmake)

init_debug()
add_openmp()

set(LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs")

add_subdirectory(${LITBASE_ROOT} litbase)
add_subdirectory(litaudioplayer)
add_subdirectory(litaudioplayer_ui)
add_subdirectory(litwidgets)
