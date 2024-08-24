# Add test sources
file(GLOB ARRAY_TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.c
)

add_executable(array_test ${ARRAY_TEST_SOURCES})

target_compile_options(array_test PRIVATE -g -Wall -Wextra)

target_link_libraries(array_test PRIVATE ${CHECK})

enable_testing()

add_test(NAME array_test COMMAND array_test)

# Setting target to expand macros

## Collect include directories from the parent scope
get_property(INCLUDE_DIRECTORIES DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)


## Create a list to store expanded files
set(EXPANDED_FILES)

add_custom_target(expand_macros)

foreach(SOURCE_FILE ${ARRAY_TEST_SOURCES})
  ### Get the directory of the source file
  get_filename_component(DIR ${SOURCE_FILE} DIRECTORY)

  ### Get the filename without the extension
  get_filename_component(FILENAME_WE ${SOURCE_FILE} NAME_WE)

  ### Define the expanded file path
  set(EXPANDED_FILE ${DIR}/${FILENAME_WE}_expanded.c)

  ### Construct the include directory arguments
  set(INCLUDE_FLAGS "")
  foreach(INC_DIR ${INCLUDE_DIRECTORIES})
    list(APPEND INCLUDE_FLAGS "-I${INC_DIR}")
  endforeach()

  ### Add the expanded file to the list
  list(APPEND EXPANDED_FILES ${EXPANDED_FILE})

  ### Add a custom command to expand macros for each file
  add_custom_command(
    OUTPUT ${EXPANDED_FILE}
    COMMAND ${CMAKE_C_COMPILER} -E -P ${INCLUDE_FLAGS} ${SOURCE_FILE} -o ${EXPANDED_FILE}
    DEPENDS ${SOURCE_FILE}
    COMMENT "Expanding macros in ${SOURCE_FILE} -> ${EXPANDED_FILE}"
    COMMAND_EXPAND_LISTS
  )

  ### Make the expand_macros target depend on this command
  add_custom_target(expand_${FILENAME_WE} DEPENDS ${EXPANDED_FILE})
  add_dependencies(expand_macros expand_${FILENAME_WE})
endforeach()

## Custom target to clean up expanded files
add_custom_target(clean_expanded_files
    COMMAND ${CMAKE_COMMAND} -E rm -f ${EXPANDED_FILES}
    COMMENT "Cleaning up expanded files"
)

## Ensure the clean_expanded_files target is invoked when "make clean" is run
set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${EXPANDED_FILES})
