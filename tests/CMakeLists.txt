enable_testing()

# State tests
add_executable(state-tests CubeStateTests.test.cpp)
target_link_libraries(state-tests gtest_main lib_cube-solver)
add_test(state-gtests state-tests state-gtests)
