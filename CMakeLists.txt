cmake_minimum_required(VERSION 3.14)
project(crow_rest_api)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Can be loaded by conan_toolchain.cmake
find_package(SQLite3 REQUIRED)

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

# Download Crow using FetchContent - no conan package available
include(FetchContent)
FetchContent_Declare(
    crow
    GIT_REPOSITORY https://github.com/CrowCpp/Crow
    GIT_TAG master
)
FetchContent_MakeAvailable(crow)

# Add source files
add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/api_server.cpp
    src/request_handler.cpp
    src/database_manager.cpp
    src/utils.cpp
)

# Add include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${crow_SOURCE_DIR}/include
    ${SQLite3_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    SQLite::SQLite3
    Crow::Crow
)

# Install target
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    CONFIGURATIONS Debug Release
)