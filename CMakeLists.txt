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
set(OUTPUT_DIR "${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}-${ARCHITECTURE}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/${OUTPUT_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/${OUTPUT_DIR})


# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# Add external libraries
add_subdirectory(vendor/GLFW)
add_subdirectory(vendor/imgui)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/Simulation)
include_directories(C:/dev/vcpkg/installed/x64-windows/include) # I will not do this the right way
#find_package(boost) # If it works for u

add_executable(Silva
    "Silva.cpp"
    "Silva.h"

    # add other source files here
     
    "src/ImGui/App.h"
    "src/ImGui/App.cpp"
    "vendor/csv/csv.h"
    "src/ImGui/imgui_impl_glfw.cpp"
    "src/ImGui/imgui_impl_glfw.h"
    "src/ImGui/imgui_impl_opengl3.cpp"
    "src/ImGui/imgui_impl_opengl3.h"
)

target_include_directories(Silva PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Simulation
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/GLFW/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/csv

)

target_link_libraries(Silva PUBLIC
    glfw
    ImGui
    opengl32
    Simulation
)