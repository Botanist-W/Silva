# CMakeList.txt : CMake project for Silva, include source and define
# project specific logic here.
cmake_minimum_required (VERSION 3.20)

cmake_policy(SET CMP0091 NEW)

message(STATUS "
    CONFETTI party model
    Simulation to understand the impact of conspecific negative density dependence in a fragmented landscape 
    University of Sheffield 
    William George &
      Prof. Robert Freckleton 
    Masters project 2023 - 24
    https://github.com/Botanist-W/Main-simulation
")

project(Silva)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Output directories
set(ARCHITECTURE "x64")  # Define architecture if not already defined
set(OUTPUT_DIR "${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}-${ARCHITECTURE}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${OUTPUT_DIR})

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

add_subdirectory(vendor/GLFW)
add_subdirectory(vendor/imgui)
add_subdirectory(vendor/spdlog)
#find_package(Boost REQUIRED) ## I wish!!

file(GLOB_RECURSE MY_SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
)

# Add the precompiled header
set(PCH_HEADER "${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h")
set(PCH_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/pch.cpp")


add_executable(Silva ${MY_SOURCES} ${PCH_SOURCE})

target_compile_definitions(Silva PUBLIC RESOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/data/")

target_precompile_headers(Silva PRIVATE ${PCH_HEADER})

# Include directories and link libraries
target_include_directories(Silva PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
     ${CMAKE_CURRENT_SOURCE_DIR}/vendor/boost #find_package would not work because my computer is shite
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLFW/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/include
)

target_link_libraries(Silva PUBLIC
    glfw
    ImGui
    opengl32
    spdlog
)
