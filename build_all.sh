#!/bin/bash

# Default build type
BUILD_TYPE=${1:-Release}

# Validate build type
if [[ "${BUILD_TYPE}" != "Release" && "${BUILD_TYPE}" != "Debug" ]]; then
    echo "Error: Build type must be either 'Release' or 'Debug'"
    echo "Usage: $0 [Release|Debug]"
    exit 1
fi

echo "Building in ${BUILD_TYPE} mode..."

# Create build directory with build type
BUILD_DIR="build/${BUILD_TYPE,,}"
mkdir -p "${BUILD_DIR}/conan"

# Navigate to build/conan and run conan install
cd "${BUILD_DIR}/conan"
conan install ../../.. --build=missing -of=. -s build_type=${BUILD_TYPE}

# Navigate back to build directory
cd ..

# Create a local installation directory within the project
INSTALL_PREFIX="./local_install"
mkdir -p "$INSTALL_PREFIX"

# Run CMake configuration
cmake ../.. \
    -DCMAKE_TOOLCHAIN_FILE=./conan/conan_toolchain.cmake \
    -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
    -DCMAKE_INSTALL_PREFIX="$INSTALL_PREFIX"

# Build the solution
cmake --build .

# Install the built solution
cmake --install .

echo "Installation complete. Build type: ${BUILD_TYPE}"
echo "Executables are in ${BUILD_DIR}/$INSTALL_PREFIX/bin"
echo "Libraries are in ${BUILD_DIR}/$INSTALL_PREFIX/lib"
echo "You may need to add $(pwd)/$INSTALL_PREFIX/bin to your PATH to run the executable."