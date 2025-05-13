cmake_minimum_required(VERSION 3.23)
set(project "window")
project(${project})

set(CMAKE_CXX_STANDARD 20)

set(${project}_sources
    window.cpp
)

set(${project}_headers
    window.hpp
)

add_library(${project}
    STATIC
    ${${project}_sources}
    ${${project}_headers}
)
