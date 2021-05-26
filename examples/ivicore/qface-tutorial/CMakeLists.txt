# Generated from qface-tutorial.pro.
cmake_minimum_required(VERSION 3.14)
project(qface-tutorial LANGUAGES CXX)

add_subdirectory(instrument-cluster)
add_subdirectory(chapter1-basics)
add_subdirectory(chapter2-enums-structs)
add_subdirectory(chapter3-simulation-backend)
add_subdirectory(chapter4-simulation-behavior)
add_subdirectory(chapter5-ipc)
if(TARGET Qt::DBus)
    add_subdirectory(chapter6-own-backend)
endif()
