cmake_minimum_required(VERSION 3.15)
include(cmake/ToolChain.cmake)

# Change the project name
project(example CXX C)

add_library(project_options INTERFACE)

# Generate compile_commands.json to make it easier to work with clang based tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Colors is unsupported vscode, can also be turend off in cmake/StandardProjectSettings.cmake
target_compile_options(project_options INTERFACE -fdiagnostics-color=never)

add_library(project_warnings INTERFACE)


option(ENABLE_IPO "Enable Interprocedural Optimization, aka Link Time Optimization (LTO)" OFF)
if(ENABLE_IPO)
  set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

option(ENABLE_PCH "Enable Precompiled Headers" OFF)
if(ENABLE_PCH)
  target_precompile_headers(
    project_options
    INTERFACE
    <vector>  # Insert preferred headers
    <string>
    <map>
    <utility>)
endif()

add_subdirectory(modm)

add_executable(${CMAKE_PROJECT_NAME}
  main.cpp)

# Add include files here
target_include_directories(${CMAKE_PROJECT_NAME}
  PRIVATE
  include)

# Extra compiler options for local project.
#target_compile_options(${CMAKE_PROJECT_NAME}
#  PRIVATE
#  -Wpedantic
#  -Wconversion
#)


target_link_libraries(${CMAKE_PROJECT_NAME}
  project_options
#  modm_arch_options
  modm_options
  modm_warnings
  modm)

# Outputs hex and bin files.
