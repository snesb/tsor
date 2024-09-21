cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(
    tsor
    LANGUAGES C CXX
    VERSION "1.0"
    DESCRIPTION "MPEG Transport Stream Inspector"
)

set(CMAKE_BUILD_TYPE "Debug") # "Release" or "Debug"
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

file(GLOB_RECURSE tsor-cpps src/*.cpp)
add_executable(tsor ${tsor-cpps})

add_subdirectory(src/util)
include_directories(include)
link_directories(lib)

find_package(cxxopts "3.1.1" REQUIRED)
link_libraries(${CXXOPTS_LIBRARY})
