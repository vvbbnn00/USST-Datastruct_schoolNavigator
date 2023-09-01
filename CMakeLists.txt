cmake_minimum_required(VERSION 3.26)
project(schoolNavigator C)

set(CMAKE_C_STANDARD 23)

add_executable(schoolNavigator main.c
        avl.h
        models.h
        math_ext.h
        data.h
        vector.h
        dijkstra.h)
