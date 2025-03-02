#!/bin/bash

# Check if project name is provided
if [ -z "$1" ]; then
    echo "Usage: $0 <project_name>"
    exit 1
fi

# Assign input project name
project_name=$1

# Define the content of the CMakeLists.txt file
cmake_content=$(cat <<EOL
# CMakeLists.txt
cmake_minimum_required(VERSION 3.10) # Specify the minimum version of CMake
project(${project_name} C)      # Define the project name and language

# Set the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Add compile_options for debug information
add_compile_options(-g)

# Specify the source files and header files
set(SOURCES
    ./src/main.c
)

# set(HEADERS
#     ./src/header.h
# )

# Create an executable target
add_executable(${project_name} \${SOURCES})

# Optionally, specify include directories (e.g., if you have headers in other directories)
# target_include_directories(${project_name} PRIVATE src)

# Add custom commands for cleaning up files
# CMake handles cleaning up build files automatically, so no need for custom commands.
EOL
)

# Create the necessary directory structure
mkdir -p src

# Create the CMakeLists.txt file
echo "$cmake_content" > CMakeLists.txt

# Create a sample source file to avoid CMake errors about missing files
echo "int main() { return 0; }" > src/main.c

echo "Boilerplate CMakeLists.txt and source file created in src directory."
