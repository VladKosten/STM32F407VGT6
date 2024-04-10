add_subdirectory(config)

# Add source
file(GLOB CoreSource *.c)

# Add HAL library
add_library(Core STATIC ${CoreSource})

# Add include directories
target_include_directories(Core PUBLIC include)

# Add CMSIS library
target_link_libraries(Core PUBLIC HAL)

message(STATUS "HAL library added")
message(STATUS "LL  library added")
