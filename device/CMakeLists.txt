

# Add the startup script and linker script to the build
add_library(Device STATIC ${CMAKE_CURRENT_SOURCE_DIR}/system_stm32f4xx.c)

# Add definitions and include directories
target_compile_definitions(Device PUBLIC ${MCU_SERIES})

# Add the include directory
target_include_directories(Device PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Include)

# Link needed libraries
target_link_libraries(Device PUBLIC Cmsis
                             PRIVATE CompileOptions)


