# Add Cmsis library as an interface library to the project so that it can be used by other targets in the project
add_library(Cmsis INTERFACE)

target_include_directories(Cmsis INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)


message(STATUS "Cmsis library added to the project")

