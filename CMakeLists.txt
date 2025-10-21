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

# # Add a custom command to generate disassembly after building the executable
# foreach(SRC_FILE ${SRC})
#     get_filename_component(BASE_NAME ${SRC_FILE} NAME_WE)

#     add_custom_command(
#         OUTPUT ${CMAKE_BINARY_DIR}/${BASE_NAME}.s
#         COMMAND ${CMAKE_CXX_COMPILER} 
#             -O3 
#             -std=c++20
#             -fverbose-asm 
#             -march=native
#             -I ${CMAKE_SOURCE_DIR}/include
#             -S ${CMAKE_SOURCE_DIR}/${SRC_FILE} 
#             -o ${CMAKE_BINARY_DIR}/${BASE_NAME}.s
#         DEPENDS ${CMAKE_SOURCE_DIR}/${SRC_FILE}
#         COMMENT "Generating assembly for ${SRC_FILE}"
#         VERBATIM
#     )

#     list(APPEND ASM_FILES ${CMAKE_BINARY_DIR}/${BASE_NAME}.s)
# endforeach()

# message(${ASM_FILES})
# add_custom_target(disassembly ALL DEPENDS ${ASM_FILES})

# Add tests
add_subdirectory(tests)