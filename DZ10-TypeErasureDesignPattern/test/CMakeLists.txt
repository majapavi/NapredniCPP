add_executable(do_tests do_tests.cpp ../myfunction.h)
target_link_libraries(do_tests gtest_main)

set_target_properties(do_tests
    PROPERTIES
    CMAKE_CXX_STANDARD 20
    )

add_test(NAME do_tests COMMAND do_tests)
