cmake_minimum_required(VERSION 3.10)
project(AbsoluteCinema)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(AbsoluteCinema
        main.cpp
        Graph.cpp
        DataFormat.cpp
        Graph.h
        DataFormat.h
        BFS.h
        BFS.cpp
)


file(COPY "IMDb movies.csv" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})