# Core library CMake configuration
project(restapi_core)

# Find required packages
find_package(SQLite3 REQUIRED)
find_package(Crow REQUIRED)

# Create library
add_library(${PROJECT_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/api_server.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/request_handler.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/database_manager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils.cpp
)

# Include directories
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link dependencies
target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Crow::Crow
    PRIVATE
        SQLite::SQLite3
)

# Install library
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

# Install headers
install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.hpp"
)
