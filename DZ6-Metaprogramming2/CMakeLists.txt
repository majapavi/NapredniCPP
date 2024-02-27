cmake_minimum_required(VERSION 3.13)
project(meta)

#find_package(fmt)
include(FetchContent)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt
   GIT_TAG   10.1.1
)
FetchContent_MakeAvailable(fmt)
add_executable(meta main.cpp)
target_link_libraries(meta fmt::fmt-header-only)

set_target_properties(
    meta
    PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED ON
)