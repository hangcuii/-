cmake_minimum_required(VERSION 3.26)
project(untitled16)


find_package(raylib)
add_executable(untitled16 main.c
        wuziqi.h
)
target_link_libraries(${PROJECT_NAME} raylib)