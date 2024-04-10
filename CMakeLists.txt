# First, we need to check the version of CMake. If the version is less than 3.12, we need to stop the compilation process.
cmake_minimum_required(VERSION 3.15)

# Include the CMake ARM GCC toolchain file.
if(NOT COMPILER)
    include(gcc-arm-none-eabi.cmake)
    message(STATUS "Compiler: arm-none-eabi-gcc")
elseif(COMPILER STREQUAL "GCC")
    include(gcc-arm-none-eabi.cmake)
    message(STATUS "Compiler: ${COMPILER}")
else()
    # Need add clang compiler support
    message(FATAL_ERROR "The compiler is not supported.")
endif()


# Set the project name and enable the C and ASM languages.
project(App C ASM)
message(STATUS "Project Name: ${CMAKE_PROJECT_NAME}")

# Set the C standard to C99 (Not required, but it is a good practice).
set(CMAKE_C_STANDARD 99)

# Set the MCU parameters.
if(NOT MCU_MODEL)
    set(MCU_FAMILY STM32F4)
    set(MCU_SERIES STM32F407xx)
    set(MCU_MODEL STM32F407VG)
elseif(MCU_MODEL STREQUAL STM32F407VG)
    set(MCU_FAMILY STM32F4)
    set(MCU_SERIES STM32F407xx)
endif()

message(STATUS "MCU Family: ${MCU_FAMILY}")
message(STATUS "MCU Series: ${MCU_SERIES}")
message(STATUS "MCU Model: ${MCU_MODEL}")

# Set the startup file and linker script based on the MCU model. And set specific compiler options based on the MCU model.
# STM32F407VG have cortex-m4 core with FPU. So, we need to set the FPU type and the floating-point ABI.
if(MCU_SERIES STREQUAL STM32F407xx)
    set(STARTUP_SCRIPT ${STARTUP_SCRIPT_STM32F407})
    set(LINKER_SCRIPT ${LINKER_SCRIPT_STM32F407VGT})
    set(MCU_PARAMETER  -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mcpu=cortex-m4)
endif()

message(STATUS "Startup Script: ${STARTUP_SCRIPT}"
                                "Linker Script: ${LINKER_SCRIPT}"
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
                                                ${MCU_PARAMETER})               # set the floating-point ABI)

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
target_link_options(CompileOptions INTERFACE -T${LINKER_SCRIPT}                 # set the linker script
                                             -Wl,-Map=${CMAKE_PROJECT_NAME}.map # generate a map file
                                             -Wl,--gc-sections                  # enable garbage collection of unused input sections
                                             -Wl,--start-group                  # start a group of input sections
                                             --specs=nosys.specs                # use the nosys.specs file
                                             --specs=nano.specs                 # use the nano.specs file
                                             -Wl,--end-group                    # end a group of input sections
                                             -Wl,--print-memory-usage           # print memory usage
                                             -mthumb                            # Link for the Thumb instruction set
                                             ${MCU_PARAMETER})

# Add the subdirectories to the project to compile the source code.
# add_subdirectory(app)
add_subdirectory(core)
add_subdirectory(device)
add_subdirectory(driver)
add_subdirectory(rtos)

# Add the executable target to the project.
add_executable(${CMAKE_PROJECT_NAME} ${STARTUP_SCRIPT})

# Add the library targets to the executable target.
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE CompileOptions
                                                    Core)