cmake_minimum_required(VERSION 3.20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
enable_testing()
set(CMAKE_COLOR_DIAGNOSTICS ON)

set(EXE_NAME cube-solver)
project(${EXE_NAME} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SRC
        src/main.cpp
        src/Cube.cpp
        src/CfopSolver.cpp
)

set(HEADERS
        include/Cube.hpp
        include/CubeSolver.hpp
        include/Timer.hpp
)

# External libs
include(FetchDependencies.cmake)

add_executable(${EXE_NAME} ${SRC} ${HEADERS})
add_library(lib_${EXE_NAME} ${SRC} ${HEADERS})

target_include_directories(${EXE_NAME} PUBLIC include)
target_include_directories(lib_${EXE_NAME} PUBLIC include)

# Add tests
add_subdirectory(tests)