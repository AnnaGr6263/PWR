cmake_minimum_required(VERSION 3.15)

# Set the project name and output path
project(zad3)

# Set the executable output path
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

# Source files
file(GLOB SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Add executable
add_executable(zad3 ${SOURCE})

# Set the output directory for plots
set(OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plots)
target_compile_definitions(zad3 PRIVATE OUTPUT_DIRECTORY="${OUTPUT_DIRECTORY}")

# Create the output directory for plots
file(MAKE_DIRECTORY ${OUTPUT_DIRECTORY})

# Suppress warnings for the project
target_compile_options(zad3 PRIVATE -w)

# Add Matplot++ as a subdirectory
add_subdirectory(matplot)

# Suppress warnings for Matplot++
target_compile_options(matplot PRIVATE -w)

# Link Matplot++ to the project
target_link_libraries(zad3 PUBLIC matplot)
