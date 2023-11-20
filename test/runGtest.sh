#!/bin/bash
# BASH script to `cmake` a unit test to pull in GTest, compile source, and run. Source from:
# https://www.eriksmistad.no/getting-started-with-google-test-on-ubuntu/?fbclid=IwAR0EFwGwosIMM4m2nmDXp2c3d-De1AyAmkl_8LsYONzlYJ3LJ_1fRUZkBEA

echo "Entering runGtest.sh"

if ! test -d ./CMakeFiles; then
    echo "Performing first-time CMAKE..."
    cmake CMakeLists.txt
    echo "First-time CMAKE DONE"
fi
make && echo "================================================================================" && \
./testMain $1
