cmake_minimum_required(VERSION 3.14)
project(crow_rest_api)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enhanced build type configuration
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Set compile options based on build type
if(CMAKE_BUILD_TYPE MATCHES Debug)
    message(STATUS "Debug build - adding debug flags")
    add_compile_options(-g3 -ggdb -O0 -Wall -Wextra -Wpedantic -fno-omit-frame-pointer)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG")
else()
    message(STATUS "Release build - optimizing")
    add_compile_options(-O3)
endif()

# Set single output directory for all build types
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# Add the libs subdirectory if it exists
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/libs/CMakeLists.txt")
    add_subdirectory(libs)
else()
    message(FATAL_ERROR "libs/CMakeLists.txt not found. Please ensure the directory structure is correct.")
endif()

# Main executable
add_executable(${PROJECT_NAME} 
    src/main.cpp
)

# Add include directories for the executable
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/restapi_core/include
)

# Link with the core library
target_link_libraries(${PROJECT_NAME} PRIVATE
    restapi_core
)

# Install target
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    CONFIGURATIONS Debug Release
)