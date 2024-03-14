cmake_minimum_required(VERSION 3.16.3)

project(BrainfuckInterpreter C)

add_subdirectory("${PROJECT_SOURCE_DIR}/src")

set(RESOURCE_DIR "${PROJECT_SOURCE_DIR}/res")

configure_file(constants.h.in "${PROJECT_SOURCE_DIR}/src/constants.h" @ONLY)