# Enable CMake FetchContent module to download FreeRTOS kernel
# from github and make it available to the project as a library
include(FetchContent)
FetchContent_Declare( freertos_kernel
  GIT_REPOSITORY https://github.com/FreeRTOS/FreeRTOS-Kernel.git
  GIT_TAG        V10.4.3-LTS-Patch-3
)

# Select the heap port
message(STATUS "Setting FREERTOS_HEAP = 4")
set(FREERTOS_HEAP "4" CACHE STRING "" FORCE)
# Select the native compile PORT
message(STATUS "UNIX, setting FREERTOS_PORT = GCC_ARM_CM4F")
set(FREERTOS_PORT "GCC_ARM_CM4F" CACHE STRING "" FORCE)
# And make it available
FetchContent_MakeAvailable(freertos_kernel)