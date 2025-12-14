cmake_minimum_required(VERSION 4.1)

project(chip-8 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23 CACHE STRING "C++ standard version")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE BOOL "Export compilation commands" FORCE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(thirdparty/SDL EXCLUDE_FROM_ALL)

add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/instruction_set.cpp
    src/chip_8.cpp
)

target_include_directories(${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE SDL3::SDL3
)