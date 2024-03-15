cmake_minimum_required(VERSION 3.16.3)

add_executable(brainfuckInterpreter main.c)

add_subdirectory(Util)

target_link_libraries(brainfuckInterpreter PRIVATE Util)
target_include_directories(brainfuckInterpreter PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Util)

add_subdirectory(Interpreter)

target_link_libraries(brainfuckInterpreter PRIVATE Interpreter)
target_include_directories(brainfuckInterpreter PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Interpreter)

target_compile_options(brainfuckInterpreter PRIVATE -Wall)