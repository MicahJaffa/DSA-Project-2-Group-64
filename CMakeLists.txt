cmake_minimum_required(VERSION 3.10)
project(AbsoluteCinema LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MINGW)
    get_filename_component(mingw_bin "${CMAKE_CXX_COMPILER}" DIRECTORY)
    file(GLOB mingw_runtime "${mingw_bin}/libgcc_s_*-1.dll" "${mingw_bin}/libstdc++-6.dll" "${mingw_bin}/libwinpthread-1.dll")
    file(COPY ${mingw_runtime} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
endif()

add_library(cinema_graph graph.cpp DataFormat.cpp)
target_include_directories(cinema_graph PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

add_executable(AbsoluteCinema main.cpp)
target_link_libraries(AbsoluteCinema PRIVATE cinema_graph)

configure_file("IMDb movies.csv" "IMDb movies.csv" COPYONLY)

include(CTest)
if(BUILD_TESTING)
    add_executable(connection_tests tests/connections.cpp)
    target_link_libraries(connection_tests PRIVATE cinema_graph)
    add_test(NAME example_connections COMMAND connection_tests example "${CMAKE_CURRENT_SOURCE_DIR}/tests/movies.csv")
    add_test(NAME imdb_connections COMMAND connection_tests imdb "${CMAKE_CURRENT_BINARY_DIR}/IMDb movies.csv")
endif()
