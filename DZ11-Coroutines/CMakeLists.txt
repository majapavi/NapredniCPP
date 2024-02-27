cmake_minimum_required(VERSION 3.18)
project (parser)

# parser implementiran pomoću obične funkcije 
add_executable(old parser_classic.cpp)

# parser implementiran pomoću korutine. Završna verzija
add_executable(main main.cpp)

set_target_properties(old  main
  PROPERTIES
    CXX_STANDARD 23
    CXX_EXTENSIONS OFF
    CXX_STANDARD_REQUIRED ON
  )

if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -W -Wall -Wno-missing-field-initializers -fconcepts-diagnostics-depth=4 -ftemplate-backtrace-limit=0")
endif()
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -W -Wall -Werror ")
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNOMINMAX")
endif()  

#target_compile_options(main  PRIVATE "-fcoroutines")
