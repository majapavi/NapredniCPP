add_executable(utest utest.cpp ../src/array.h)
target_link_libraries(utest  gtest_main)

add_test(NAME utest COMMAND utest)
