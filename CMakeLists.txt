cmake_minimum_required(VERSION 3.23)

set(project "Sudoku")
project(${project})

set(CMAKE_CXX_STANDARD 20)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/sources/Data.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

set(${project}_SOURCES
    sources/main.cpp
    sources/game.cpp
)

set(${project}_HEADERS
    sources/game.h
)

add_subdirectory(sudokuLib)

add_executable(${project}
    ${${project}_SOURCES}
    ${${project}_HEADERS}
)

target_link_libraries(${project} sudokuLib)
