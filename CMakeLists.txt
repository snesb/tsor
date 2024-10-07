cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

# Find cppcheck for static code analysis
find_program(CXX_CPPCHECK cppcheck NAMES cppcheck HINTS $ENV{PROGRAMFILES}/cppcheck)
if(CXX_CPPCHECK)
    list(APPEND CXX_CPPCHECK
        "--quiet" "--std=c++${CMAKE_CXX_STANDARD}" "--enable=all"
        "--inline-suppr" "--suppress=missingInclude" "--suppress=missingIncludeSystem" "src"
    )

    add_custom_target(check
        COMMAND ${CXX_CPPCHECK}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Static code analysis using cppcheck"
    )
endif()

# Setup tsor project
project(tsor
    LANGUAGES C CXX
    VERSION "0.1"
    DESCRIPTION "MPEG Transport Stream Inspector"
)
include_directories(include lib/cxxopts/include src/ts)
link_directories(lib)

# Add tsor source files
set(TSOR_SOURCES
    ${PROJECT_SOURCE_DIR}/src/tsor.cpp
    ${PROJECT_SOURCE_DIR}/src/gui/gui.cpp
    ${PROJECT_SOURCE_DIR}/src/util/options.cpp
)
add_subdirectory(src/ts)
add_subdirectory(src/util)
add_executable(tsor ${TSOR_SOURCES})
if(CXX_CPPCHECK)
    add_dependencies(tsor check)
endif()

# Find dependency packages
find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

# Add Dear ImGui library from submodule
set(IMGUI_PATH lib/imgui)
file(GLOB IMGUI_SOURCES
    ${IMGUI_PATH}/*.cpp
    ${IMGUI_PATH}/backends/imgui_impl_glfw.cpp
    ${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp
)
add_library(ImGui STATIC ${IMGUI_SOURCES})
target_include_directories(ImGui PUBLIC ${IMGUI_PATH} ${IMGUI_PATH}/backends)

# Link tsor
target_link_libraries(tsor ts ${CXXOPTS_LIBRARY} OpenGL::GL glfw ImGui)
