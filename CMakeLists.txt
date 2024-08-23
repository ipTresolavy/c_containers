cmake_minimum_required(VERSION 3.10)

# Define the project
project(CContainers C)

# Set the C standard globally (applies to all targets)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/tests/include
)

# Source files (use GLOB to simplify adding multiple files)
#file(GLOB SOURCES
#    ${PROJECT_SOURCE_DIR}/src/*.c
#)

# Main executable (for the main project, not the tests)
#add_executable(main_executable ${SOURCES})

# Set compiler options for the main target
#target_compile_options(main_executable PRIVATE -Wall -Wextra)

# Option to enable/disable building tests
option(BUILD_TESTING "Build tests using the Check framework" OFF)

# Conditional inclusion of the test CMakeLists.txt
if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()

