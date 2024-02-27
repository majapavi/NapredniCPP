# Kod postavljanje minimalne trazene verzije na 3.14 dobivam deprication warning
cmake_minimum_required(VERSION 3.5)
project(array LANGUAGES CXX)
enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  # Koristi se starija verzija, kakva je koristena i u ex5 u CMake vjazbama, jer nova nije radila
  GIT_TAG     e2239ee6043f73722e7aa812a459f54a28552929  # release-1.11.0
#  GIT_TAG    v1.14.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_subdirectory(src)
add_subdirectory(test)
