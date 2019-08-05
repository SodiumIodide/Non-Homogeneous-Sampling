cmake_minimum_required(VERSION 2.6)
project(GeometryPrint)

add_compile_options(-O3 -Wall -Wextra -pedantic -Werror)

set(SRCS geometry_print.c)

add_executable(GeometryPrint "${SRCS}")

target_link_libraries(GeometryPrint m)
