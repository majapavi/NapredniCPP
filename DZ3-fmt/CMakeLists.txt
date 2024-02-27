cmake_minimum_required(VERSION 3.5)
project(fmt_project CXX)
add_subdirectory(src)
#set(FMT_USE_STATIC_LIBS 1)
#find_package(fmt REQUIRED)

include(FetchContent)
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt
#   GIT_TAG   10.1.1
    GIT_TAG   9.1.0
)
FetchContent_MakeAvailable(fmt)