cmake_minimum_required(VERSION 3.28)
project(CppDynamicEcosystem LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

include(FetchContent)

# SFML
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

# Catch2
FetchContent_Declare(Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.5.2)
FetchContent_MakeAvailable(Catch2)

# Main program
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
include_directories(headers)

add_executable(simulation ${SOURCES})
target_compile_features(simulation PRIVATE cxx_std_17)
target_link_libraries(simulation PRIVATE SFML::Graphics SFML::Window SFML::System)

# Tests - Include both test files and source files (excluding main.cpp)
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS tests/*.cpp)
file(GLOB_RECURSE LIB_SOURCES CONFIGURE_DEPENDS src/*.cpp)

# Remove main.cpp from library sources if it exists
list(FILTER LIB_SOURCES EXCLUDE REGEX ".*main\\.cpp$")

add_executable(tests ${TEST_SOURCES} ${LIB_SOURCES})
target_include_directories(tests PRIVATE headers)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_compile_features(tests PRIVATE cxx_std_17)

include(CTest)
include(Catch)
catch_discover_tests(tests)