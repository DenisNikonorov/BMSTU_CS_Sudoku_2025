cmake_minimum_required(VERSION 3.23)
set(project "Sudoku")
project(${project})

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(sudokuLib)
add_subdirectory(window)
add_subdirectory(menuWindow)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

add_executable(${project} main.cpp)

target_link_libraries(${project} window)
target_link_libraries(${project} menuWindow)
target_link_libraries(${project} sudokuLib)
target_link_libraries(${project} sfml-graphics)
