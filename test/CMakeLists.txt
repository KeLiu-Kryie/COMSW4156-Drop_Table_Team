# Source from:
# https://www.eriksmistad.no/getting-started-with-google-test-on-ubuntu/?fbclid=IwAR0EFwGwosIMM4m2nmDXp2c3d-De1AyAmkl_8LsYONzlYJ3LJ_1fRUZkBEA

cmake_minimum_required(VERSION 3.5)
project(DropTableTeamUnitTests)
 
# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
find_package( Threads )

# For UT Code Coverage
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -g -O0 -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

# List all the source files
set(SRC_FILES
    ../src/DatabaseHandler.cpp
    ../src/TranslationOutput.cpp
    ../src/Translations.cpp
    ../src/Translator.cpp
    ../src/ocr.cpp
    ../src/pdftotext.cpp
)

# Link testMain with what we want to test
add_executable(testMain
    testMain.cpp
    ${SRC_FILES})

# Equivalent of INCLUDE
target_include_directories(testMain
    PUBLIC
    ../src
    ../include
    /usr/local/include/mongocxx/v_noabi
    /usr/local/include/bsoncxx/v_noabi
    /usr/include/poppler
    /usr/include/poppler/cpp
    /usr/include/glib-2.0
    /usr/lib/x86_64-linux-gnu/glib-2.0/include
    /usr/include/poppler)

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
    poppler-cpp
    glib-2.0
    tesseract
    lept
    poppler-cpp)

# Equivalent of CXX_FLAGS
target_compile_options(testMain PRIVATE -Wno-deprecated-declarations --coverage)
