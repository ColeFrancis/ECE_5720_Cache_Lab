# Cache Lab CMake build
# Author: Nick Waddoups
# Date: Oct 22, 2025

# --- Setup ---
cmake_minimum_required(VERSION 3.26.5)
project(cachelab C)

# Set the C standard to C11 and enforce it.
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set compiler warnings
include(cmake/CompilerWarnings.cmake)

# Set global C compiler flags used for all targets.
set(CMAKE_C_FLAGS "-g3 -pedantic -m64")

# --- Target Definitions ---

# Build trans.c as an object library.
add_library(trans_obj OBJECT trans.c)
target_compile_options(trans_obj PRIVATE -O0)
target_project_warnings(trans_obj)

# Build the csim executable.
add_executable(csim csim.c cachelab.c)
target_compile_options(csim PRIVATE -O2)
target_project_warnings(csim)

# Link the math library (-lm).
target_link_libraries(csim PRIVATE m)

# Build the test-trans executable.
add_executable(test-trans test-trans.c cachelab.c $<TARGET_OBJECTS:trans_obj>)

# Build the tracegen executable.
add_executable(tracegen tracegen.c cachelab.c $<TARGET_OBJECTS:trans_obj>)
target_compile_options(tracegen PRIVATE -O0)

# --- Completion Message ---
message(STATUS "Cache Lab CMake project configured.")
message(STATUS "Available targets: csim, test-trans, tracegen")
