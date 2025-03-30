# REST API Server

A REST API server implemented using Crow framework with SQLite backend. The project utilizes RST (REST Server Toolkit) - a custom library providing core components for building REST services. RST encapsulates common server functionality like request handling, database management, and API routing.

## Dependencies

- Crow (C++ web framework)
- SQLite3
- C++17 or higher
- RST (REST Server Toolkit) - included in libs/rest_st

## Build Instructions

1. Make sure you have Conan 2.x package manager installed
2. Choose a build configuration and follow the corresponding commands:

### Release Build (Default)
```bash
mkdir -p build/release && cd build/release
conan install ../.. --output-folder=. --build=missing
cmake ../.. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### Debug Build
```bash
mkdir -p build && cd build
conan install .. --output-folder=. --build=missing
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug
cmake --build .
```

The debug build includes additional debug symbols and compiler flags (-g3 -ggdb -O0 -Wall -Wextra -Wpedantic).

## Database

The application automatically creates an SQLite database at `data/crow_api.db` with a table structure:

```sql
CREATE TABLE data (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    value REAL NOT NULL
);
```

## API Endpoints

### GET /api/data

Retrieves all data entries.

**Response Format:**
```json
{
    "status": "success",
    "data": [
        {
            "id": 1,
            "name": "example",
            "value": 123.45
        }
    ]
}
```

### POST /api/data

Creates a new data entry.

**Request Body:**
```json
{
    "name": "example",
    "value": 123.45
}
```

**Response Format:**
```json
{
    "status": "created",
    "data": {
        "id": 1,
        "name": "example",
        "value": 123.45
    }
}
```

## Error Handling

The API returns appropriate HTTP status codes:
- 200: Successful GET request
- 201: Successful POST request
- 400: Invalid request format
- 500: Server error

Error responses include a message explaining the error:
```json
{
    "status": "error",
    "message": "Error description"
}
```

## Running the Server

The server runs on port 8080 by default. Launch the executable after building:

```bash
./bin/crow_rest_api
```

## Features

- RESTful API endpoints for data management
- Thread-safe operations using `std::shared_mutex`
- JSON request/response handling
- Atomic ID generation for data entries
- Conan package management
- CMake build system

## Building and Running

### Prerequisites
- C++17 compatible compiler (GCC 8+, Clang 7+, or MSVC 2019+)
- CMake 3.14 or higher
- Conan package manager 2.x
- SQLite 3.44.2 or higher
- Git

### Build Instructions
1. Make sure you have Conan 2.x package manager installed
2. Choose a build configuration:

```bash
# Release build (default)
./build_all.sh

# Debug build
./build_all.sh Debug
```

The script handles dependency setup, build configuration, and installation to `build/<build_type>/local_install/bin`.

### Running
Launch the server (runs on port 8080):
```bash
./build/crow_rest_api
```

## API Documentation

### Get All Data
Retrieve all stored data entries.

```bash
curl -X GET http://localhost:8080/api/data
```

Response:
```json
{
    "status": "success",
    "data": [
        {
            "id": 1,
            "name": "Example data",
            "value": 42.5
        }
    ]
}
```

### Add New Data
Create a new data entry.

```bash
curl -X POST http://localhost:8080/api/data \
     -H "Content-Type: application/json" \
     -d '{"name": "New Item", "value": 123.45}'
```

Response:
```json
{
    "status": "created",
    "data": {
        "id": 2,
        "name": "New Item",
        "value": 123.45
    }
}
```

## Architecture

### Project Structure

```
crow_rest_server/
├── CMakeLists.txt          # Main build configuration
├── conanfile.txt           # Conan dependencies
├── build_all.sh            # Build automation script
├── README.md               # Project documentation
├── LICENSE                 # MIT License file
├── libs/                   # Library components
│   └── rest_st/           # REST Server Toolkit (RST) - Core library
│       ├── include/       # Public headers for RST components
│       ├── src/          # RST implementation files
│       └── CMakeLists.txt # RST build configuration
├── src/                   # Main application source
└── data/                  # Runtime data directory
```

## Namespaces

The codebase follows a hierarchical namespace organization:

- `rst`
  - Main namespace containing the server wrapper class `ApiServer`
  - Provides the primary application interface
  - Coordinates core components and server lifecycle

- `rst::core`
  - Contains core infrastructure components:
    - Data structures (`Data`, etc.)
    - Interfaces (`IDatabaseManager`, `IRequestHandler`)
    - Database management implementation
    - Request handling logic

- `rst::utils`
  - Utility functions for data serialization and response formatting
  - Helper functions shared across components
  
## Troubleshooting

### Common Issues

1. **CMake can't find Conan packages**
   - Ensure you're using Conan 2.x
   - Run `conan profile detect` to create a default profile
   - Make sure to use the correct toolchain file path

2. **Database access errors**
   - Ensure write permissions in the `data` directory
   - Check if SQLite3 is properly installed
   - Verify database file isn't locked by another process

3. **Server won't start**
   - Check if port 8080 is available
   - Ensure proper permissions to create network socket
   - Verify all dependencies are properly linked

For more issues, please check the GitHub issues page or create a new issue.

## Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- [Crow Framework](https://crowcpp.org) - C++ microframework for web with JSON support
- [SQLite](https://www.sqlite.org/) - Embedded SQL database engine
- [Conan](https://conan.io/) - C/C++ package manager
- [CMake](https://cmake.org/) - Build system generator

## Security

- Database is protected against SQL injection through prepared statements
- Input validation for all API endpoints
- Thread-safe operations using mutex locks
- No sensitive data exposure in error messages

