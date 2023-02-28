cmake_minimum_required(VERSION 3.16)

project(Master1-Redux
        DESCRIPTION "Toy refactoring project of someone else Master degree project"
        LANGUAGES CXX C
        VERSION 0.0.1.0)


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(extern)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic ${CMAKE_CXX_FLAGS}")

add_subdirectory(src)
add_subdirectory(docs)

