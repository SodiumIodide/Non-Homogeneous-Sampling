cmake_minimum_required(VERSION 2.6)
project(GeometryPrint)

add_executable(GeometryPrint geometry_print.c)

target_link_libraries(GeometryPrint m)
