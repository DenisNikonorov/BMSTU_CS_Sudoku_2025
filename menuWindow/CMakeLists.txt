cmake_minimum_required(VERSION 3.23)
set(project "menuWindow")
project(${project})

set(CMAKE_CXX_STANDARD 20)

set(${project}_sources
    menuWindow.cpp
)

set(${project}_headers
    menuWindow.hpp
)

add_library(${project}
    STATIC
    ${${project}_sources}
    ${${project}_headers}
)
