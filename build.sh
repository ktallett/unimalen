#!/bin/bash

# Build script for unimalen

# Create build directory
mkdir -p build
cd build

# Configure with CMake
cmake ..

# Build the project
make -j$(nproc)

echo "Build complete. Run './unimalen' from the build directory to start the application."