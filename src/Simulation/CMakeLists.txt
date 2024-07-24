cmake_minimum_required(VERSION 3.20)
project(Simulation)

file(GLOB SIM_SOURCES
    "*.cpp"
    "*.h"
)


add_library(Simulation STATIC ${SIM_SOURCES})
include_directories(C:/dev/vcpkg/installed/x64-windows/include) # I will not do this the right way
#find_package(boost) # If it works for u


target_include_directories(Simulation PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
   )

