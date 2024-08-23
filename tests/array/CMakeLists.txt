# Add test sources
file(GLOB ARRAY_TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

add_executable(array_test ${ARRAY_TEST_SOURCES})

target_compile_options(array_test PRIVATE -g -Wall -Wextra)

target_link_libraries(array_test PRIVATE ${CHECK})

enable_testing()

add_test(NAME array_test COMMAND array_test)
