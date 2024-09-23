cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(
    tsor
    LANGUAGES C CXX
    VERSION "1.0"
    DESCRIPTION "MPEG Transport Stream Inspector"
)

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

include_directories(include)
link_directories(lib)

file(GLOB_RECURSE TSOR_CPPS src/*.cpp)
add_subdirectory(src/util)
add_executable(tsor ${TSOR_CPPS})

find_package(cxxopts 3.1 REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

set(IMGUI_PATH lib/imgui)
file(GLOB IMGUI_SOURCES
    ${IMGUI_PATH}/*.cpp
    ${IMGUI_PATH}/backends/imgui_impl_glfw.cpp
    ${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp
)
add_library(ImGui STATIC ${IMGUI_SOURCES})
target_include_directories(ImGui PUBLIC ${IMGUI_PATH} ${IMGUI_PATH}/backends)

target_link_libraries(tsor ${CXXOPTS_LIBRARY} OpenGL::GL glfw ImGui)
