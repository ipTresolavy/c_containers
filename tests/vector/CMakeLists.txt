set(VECTOR_TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/vector_test.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../array/test_struct.c
)

add_executable(vector_test ${VECTOR_TEST_SOURCES})

target_compile_options(vector_test PRIVATE -g -Wall -Wextra)

target_include_directories(vector_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../array)

target_link_libraries(vector_test PRIVATE ${CHECK})

enable_testing()

add_test(NAME vector_test COMMAND vector_test)
