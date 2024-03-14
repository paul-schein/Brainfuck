cmake_minimum_required(VERSION 3.16.3)

add_executable(brainfuckInterpreter brainfuckInterpreter.c)

target_include_directories(brainfuckInterpreter PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(brainfuckInterpreter PRIVATE -Wall)