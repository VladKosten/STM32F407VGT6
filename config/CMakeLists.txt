
# Add library freertos_config
add_library(freertos_config INTERFACE)

target_include_directories(freertos_config SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(freertos_config  INTERFACE   projCOVERAGE_TEST=0)

target_link_libraries(freertos_config INTERFACE Device
                                      INTERFACE CompileOptions)


# Add library config
add_library(Config INTERFACE)

target_include_directories(Config SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Config INTERFACE Device
                             INTERFACE CompileOptions)
