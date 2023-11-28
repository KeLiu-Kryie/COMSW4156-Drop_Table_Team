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

# Automatically do branch coverage with lcov. The .gcda files are
# generated during execution, and can be found in
# ./CMakeFiles/testMain.dir/<...>/*.gcda
gcda_dirs=$(find  -name *.gcda -exec dirname {} \; | sort -u)

for dir in $gcda_dirs; do
    dir_name=$(basename "$dir")
    echo "Checking coverage for $dir_name"
    lcov --directory . --capture --output-file "${dir_name}_coverage.info" #--no-external

    # Rather than using --no-external, just manually remove the dependencies from analysis
    lcov --remove "${dir_name}_coverage.info" "/usr*" -o "${dir_name}_coverage.info"
    lcov --remove "${dir_name}_coverage.info" "c++*" -o "${dir_name}_coverage.info"
    lcov --remove "${dir_name}_coverage.info" "gtest*" -o "${dir_name}_coverage.info"
    lcov --remove "${dir_name}_coverage.info" "nlohmann*" -o "${dir_name}_coverage.info"
    lcov --remove "${dir_name}_coverage.info" "poppler*" -o "${dir_name}_coverage.info"
    lcov --remove "${dir_name}_coverage.info" "tesseract*" -o "${dir_name}_coverage.info"

    genhtml "${dir_name}_coverage.info" --output-directory "${dir_name}_out"

    # Break out of this loop, since all of the directories contain copies of the same gcda files
    break
done
