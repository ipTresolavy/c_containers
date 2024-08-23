cmake_minimum_required(VERSION 3.10)

project(ArrayProject C)

# Add include directories
include_directories(include)
include_directories(tests/include)

# Source files
set(SOURCES
    tests/array/array_test.c
    tests/array/test_struct.c
)

# Main executable
add_executable(array_test ${SOURCES})

# Set the C standard to C11
set_target_properties(array_test PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    COMPILE_OPTIONS -g 
)
# Link with Check framework
target_link_libraries(array_test check)

# Set compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

# Enable testing
enable_testing()

# Add the test target
add_test(array_test array_test)

