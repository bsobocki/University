cmake_minimum_required(VERSION 3.14)
project(TDD)

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

enable_testing()

add_executable( TDD_test
   TEST_test.cpp
   src/Calculator.cpp
   src/Factorial.cpp
)

target_link_libraries(
   TDD_test
   gtest_main
)

include(GoogleTest)
gtest_discover_tests(TDD_test)

find_package(GTest REQUIRED) # Find the google testing framework on your system

include_directories( 
   src
   ${GTEST_INCLUDE_DIRS} 
)

target_link_libraries(TDD_test ${GTEST_LIBRARIES}) 