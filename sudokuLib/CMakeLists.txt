cmake_minimum_required(VERSION 3.23)

set(project "sudokuLib")
project(${project})

set(CMAKE_CXX_STANDARD 20)

set(${project}_SOURCES
    sudokuLib.cpp
)

set(${project}_HEADERS
    sudokuLib.h
)

add_library(${project}
    STATIC
    ${${project}_SOURCES}
    ${${project}_HEADERS}
)
