# We have deal with cross-compiling
set(CMAKE_SYSTEM_NAME Generic)

#Set the processor to arm
set(CMAKE_SYSTEM_PROCESSOR arm)

# Try to compile a static library to check if the compiler is working
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Set the toolchain prefix
set(TOOLCHAIN_PREFIX arm-none-eabi-)

# Set the compiler and linker
set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_SIZE ${TOOLCHAIN_PREFIX}size)
set(CMAKE_OBJDUMP ${TOOLCHAIN_PREFIX}objdump)

# Add suffix to the executables,
set(CMAKE_EXECUTABLE_SUFFIX_ASM ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C ".elf")