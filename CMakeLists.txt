cmake_minimum_required(VERSION 3.31.0)
project(Life)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # for editor using clang

include(FetchContent)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug) # probably change to release later on since its faster
endif()

FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-3.4.16
)
FetchContent_MakeAvailable(SDL3)

set(SOURCES
    src/main.cpp
)
add_executable(life ${SOURCES})

target_link_libraries(life PRIVATE SDL3::SDL3)
target_include_directories(life PRIVATE include)

if (NOT MSVC)
    target_compile_options(life PRIVATE -Wall -Wextra -Wpedantic)
endif()