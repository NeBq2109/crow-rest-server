#!/bin/bash

# Create build/conan directory
mkdir -p build/conan

# Navigate to build/conan and run conan install
cd build/conan
conan install ../.. --build=missing -of=.

# Navigate back to build directory
cd ..

# Create a local installation directory within the project
INSTALL_PREFIX="./local_install"
mkdir -p "$INSTALL_PREFIX"

# Run CMake configuration for release mode
cmake .. -DCMAKE_TOOLCHAIN_FILE=./conan/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="$INSTALL_PREFIX"

# Build the solution
cmake --build .

# Install the built solution
cmake --install .

echo "Installation complete.  Executables are in $INSTALL_PREFIX/bin. Libraries are in $INSTALL_PREFIX/lib."
echo "You may need to add $(pwd)/$INSTALL_PREFIX/bin to your PATH to run the executable."