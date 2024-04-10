# Check what the rtos is set
if(NOT RTOS)
    message( STATUS "RTOS not selected, defaulting to FreeRTOS")
    set(RTOS "FreeRTOS" CACHE STRING "RTOS to use" FORCE)
    add_subdirectory(freertos)
elseif(RTOS STREQUAL "FreeRTOS")
    message( STATUS "FreeRTOS selected")
    add_subdirectory(freertos)
endif()