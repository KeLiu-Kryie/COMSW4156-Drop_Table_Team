# Source from:
# https://www.eriksmistad.no/getting-started-with-google-test-on-ubuntu/?fbclid=IwAR0EFwGwosIMM4m2nmDXp2c3d-De1AyAmkl_8LsYONzlYJ3LJ_1fRUZkBEA

cmake_minimum_required(VERSION 3.0)
project(DropTableTeamUnitTests)
 
# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
find_package( Threads )

# Link testMain with what we want to test
add_executable(testMain
    testMain.cpp
    ../src/DatabaseHandler.cpp
    ../src/Server.cpp
    ../src/Translator.cpp)

# Equivalent of INCLUDE
target_include_directories(testMain
    PUBLIC
    ../src
    ../include
    /usr/local/include/mongocxx/v_noabi
    /usr/local/include/bsoncxx/v_noabi
    /usr/include/glib-2.0
    /usr/lib/x86_64-linux-gnu/glib-2.0/include)
    # /usr/include/poppler)

# Equivalent of LIBS
target_link_libraries(testMain
    ${GTEST_LIBRARIES}
    pthread
    crypto
    ssl
    bsoncxx
    curl
    boost_system
    mongocxx
    glib-2.0
    tesseract
    lept)
    # poppler-cpp)

# Equivalent of CXX_FLAGS
target_compile_options(testMain PRIVATE -Wno-deprecated-declarations)
