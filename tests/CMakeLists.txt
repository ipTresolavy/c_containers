find_library(CHECK check)
if(!CHECK)
  message(FATAL_ERROR "libcheck not found")
endif()

# Add the array test directory
add_subdirectory(array)
