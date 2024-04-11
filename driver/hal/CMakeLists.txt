
# Add source
file(GLOB_RECURSE HalSource source/*.c)

# Add HAL library
add_library(HAL STATIC ${HalSource})

# Add include directories
target_include_directories(HAL PUBLIC include
                               PUBLIC include/legacy)

target_compile_definitions(HAL PUBLIC USE_HAL_DRIVER)

# Add CMSIS library
target_link_libraries(HAL PUBLIC Device
                          PUBLIC Config
                          PRIVATE CompileOptions)

