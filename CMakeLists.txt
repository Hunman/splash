# The basic info of the project
cmake_minimum_required(VERSION 3.16)
project(Splash)

# Options
option(
    BUILD_TESTS
    "Fetch and build gtest as a dependency"
    ON
)
option(
    BUILD_EXAMPLES
    "Build the example"
    OFF
)

# pkg-config for dependencies
find_package(PkgConfig REQUIRED)

set(CMAKE_CXX_STANDARD 20)
file(GLOB SourceFiles
    source/*.cpp
    source/filter/*.cpp
    source/target/*.cpp
)
include_directories(include/)
add_library(Splash STATIC ${SourceFiles})

# Testing
if (BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
    )
    # For Windows: Prevent overriding the parent project's compiler/linker settings
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    find_package(GTest REQUIRED)

    add_executable(
        Test
        tests/Bitmap.cpp
        tests/Colour.cpp
    )

    target_link_libraries(
        Test
        GTest::Main
        Splash
    )

    include(GoogleTest)
    gtest_discover_tests(Test)
endif()

if (BUILD_EXAMPLES)
    include_directories(example/include)
    add_compile_definitions(Example PRIVATE DEFAULT_IMAGE="${PROJECT_SOURCE_DIR}/example/images/mountain.bmp")
    add_executable(Example example/source/main.cpp)
    target_link_libraries(Example Splash)
endif()
