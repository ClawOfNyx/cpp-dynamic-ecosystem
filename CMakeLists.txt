cmake_minimum_required(VERSION 3.28)
project(CppDynamicEcosystem LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.1
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
include_directories(headers)

add_executable(simulation ${SOURCES})
target_compile_features(simulation PRIVATE cxx_std_17)
target_link_libraries(simulation PRIVATE SFML::Graphics)