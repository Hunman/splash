# The basic info of the project
cmake_minimum_required(VERSION 3.16)
project(Splash)

# pkg-config for dependencies
find_package(PkgConfig REQUIRED)

set(CMAKE_CXX_STANDARD 20)
file(GLOB SourceFiles
    source/*.cpp
    source/filter/*.cpp
    source/target/*.cpp
)
include_directories(include/)
add_library(libSplash SHARED ${SourceFiles})
