# Source from:
# https://www.eriksmistad.no/getting-started-with-google-test-on-ubuntu/?fbclid=IwAR0EFwGwosIMM4m2nmDXp2c3d-De1AyAmkl_8LsYONzlYJ3LJ_1fRUZkBEA

cmake_minimum_required(VERSION 2.6)
 
# Locate GTest
find_package(GTest REQUIRED)
find_package( Threads )
include_directories(${GTEST_INCLUDE_DIRS})
 
# Link runTests with what we want to test and the GTest and pthread library
add_executable(testMain testMain.cpp)
target_link_libraries(testMain ${GTEST_LIBRARIES} pthread)
