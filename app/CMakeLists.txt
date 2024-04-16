# Add the startup script and linker script to the build
add_library(Application STATIC ${CMAKE_CURRENT_SOURCE_DIR}/app_startup.c)

# Add the include directory
target_include_directories(Application PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link needed libraries
target_link_libraries(Application    PUBLIC freertos_kernel
                                     PUBLIC freertos_config
                                     PUBLIC HAL
                                     PRIVATE CompileOptions)


