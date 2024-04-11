# First, we need to check the version of CMake. If the version is less than 3.12, we need to stop the compilation process.
cmake_minimum_required(VERSION 3.15)

include(gcc-arm-none-eabi.cmake)

# Set the project name and enable the C and ASM languages.
project(App C ASM)
message(STATUS "Project Name: ${CMAKE_PROJECT_NAME}")

# Set the C standard to C99 (Not required, but it is a good practice).
set(CMAKE_C_STANDARD 99)

# Set the MCU parameters.
set(MCU_FAMILY STM32F4)
set(MCU_SERIES STM32F407xx)
set(MCU_MODEL STM32F407VG)


message(STATUS "MCU Family: ${MCU_FAMILY}")
message(STATUS "MCU Series: ${MCU_SERIES}")
message(STATUS "MCU Model: ${MCU_MODEL}")

# STM32F407VG have cortex-m4 core with FPU. So, we need to set the FPU type and the floating-point ABI.
set(MCU_PARAMETER  -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mcpu=cortex-m4)

message(STATUS "Startup Script: ${STARTUP_SCRIPT_STM32F407}"
                                "Linker Script: ${LINKER_SCRIPT_STM32F407VGT}"
                                "MCU Parameter: ${MCU_PARAMETER}"   )

# Add interface library to set the compile and link options to all targets.
add_library(CompileOptions INTERFACE)

target_compile_options(CompileOptions INTERFACE -Wall                           # the compiler will output warnings for common programming errors
                                                -fdata-sections                 # place each data item into its own section in the output file
                                                -ffunction-sections             # place each function into its own section in the output file
                                                --specs=nano.specs               # use the nano.specs file
                                                -Wextra                         # enables some extra warning flags that are not enabled by -Wall
                                                -Wshadow                        # warn whenever a local variable or type declaration shadows another variable, parameter, type, class member
                                                -Wdouble-promotion              # warn if a float is implicitly promoted to a double
                                                -Wformat=2                      # check calls to printf and scanf
                                                -Wundef                         # warn if an undefined identifier is evaluated in an #if directive
                                                -fno-common                     # In C, this option controls the placement of global variables defined without an initializer
                                                -Wno-unused-variable            # do not warn about unused variables
                                                -mthumb                         # generate code for the Thumb instruction set
                                                ${MCU_PARAMETER}
                                                )               # generate code for the Thumb instruction set

# Set the compile options options based on the build type (Debug or Release)
IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(CompileOptions INTERFACE -O0      # no optimization
                                                    -g3      # generate debug information
                                                    -ggdb3)  # produce debugging information for use by GDB
ELSE ()
    target_compile_options(CompileOptions INTERFACE -O3      # optimize yet more
                                                    -g0      # no debug information
                                                    -ggdb0)  # no debugging information
ENDIF()

message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

# Set the link options.
target_link_options(CompileOptions INTERFACE -T${LINKER_SCRIPT_STM32F407VGT}   # set the linker script
                                             -Wl,--gc-sections                 # enable garbage collection of unused input sections
                                             -specs=nano.specs                 # use the nano.specs file
                                             -specs=nosys.specs                # use the nosys.specs file
                                             -Wl,--print-memory-usage          # print memory usage
                                             -mthumb                           # Link for the Thumb instruction set
                                             ${MCU_PARAMETER})                 # set the floating-point ABI)

# Add the subdirectories to the project to compile the source code.
# add_subdirectory(app)
add_subdirectory(config)
add_subdirectory(device)
add_subdirectory(driver)
# add_subdirectory(rtos)

# Add the executable target to the project.
add_executable(${CMAKE_PROJECT_NAME} main.c
                                    ${STARTUP_SCRIPT_STM32F407})


# Add the library targets to the executable target.
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE CompileOptions
                                            PRIVATE HAL)
