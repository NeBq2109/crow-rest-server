cmake_minimum_required(VERSION 3.14)
project(crow_rest_api)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add this for Conan 2.x
find_package(Boost REQUIRED)
find_package(SQLite3 REQUIRED)

# Set build output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

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

# Download Crow using FetchContent
include(FetchContent)
FetchContent_Declare(
  crow
  GIT_REPOSITORY https://github.com/CrowCpp/Crow
  GIT_TAG master
)
FetchContent_MakeAvailable(crow)

# Add source files
add_executable(${PROJECT_NAME} src/main.cpp)

# Add include directories
target_include_directories(${PROJECT_NAME} PRIVATE
  ${Boost_INCLUDE_DIRS}
  ${crow_SOURCE_DIR}/include # Path to Crow's include directory
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
  Boost::boost
  SQLite::SQLite3
  pthread
)

# Install target
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)