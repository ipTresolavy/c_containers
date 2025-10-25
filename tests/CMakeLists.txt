find_library(CHECK check)
if(!CHECK)
  message(FATAL_ERROR "libcheck not found")
endif()

# Add the array and vector test directories
add_subdirectory(array)
add_subdirectory(vector)
