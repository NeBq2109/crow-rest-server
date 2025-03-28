# REST API Server

A simple REST API server implemented using Crow framework with SQLite backend.

## Dependencies

- Crow (C++ web framework)
- SQLite3
- C++17 or higher

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

## Prerequisites

- C++17 compatible compiler (GCC 8+, Clang 7+, or MSVC 2019+)
- CMake 3.14 or higher
- Conan package manager 2.x
- Git
- SQLite 3.44.2 or higher
- At least 1GB of free disk space
- Internet connection for dependency downloads

## Building the Project

### Using the Build Script (Recommended)

The easiest way to build the project is using the provided build script. The script supports both Release and Debug builds:

```bash
# For Release build (default)
./build_all.sh

# For Debug build
./build_all.sh Debug
```

The script will:
1. Set up the Conan dependencies with the specified build type
2. Configure CMake with the correct toolchain and build type
3. Build the project
4. Install the executable to `build/<build_type>/local_install/bin`

### Build Types

- **Release** (default): Optimized build with -O3 flag
- **Debug**: Includes debug symbols and additional warning flags

## Running the Server

After building, run the server:

```bash
./build/crow_rest_api
```

The server will start on `http://localhost:8080`.

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

## Project Structure

```
crow_rest_server/
├── CMakeLists.txt          # Main build configuration
├── conanfile.txt           # Conan dependencies
├── build_all.sh            # Build automation script
├── README.md               # Project documentation
├── LICENSE                 # MIT License file
├── .vscode/               # VS Code configuration
├── include/               # Header files
├── src/                  # Source files
└── data/                 # Runtime data directory
```

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

- [Crow Framework](https://crowcpp.org) - C++ microframework for web
- [SQLite](https://www.sqlite.org/) - Embedded SQL database engine
- [Conan](https://conan.io/) - C/C++ package manager
- [CMake](https://cmake.org/) - Build system generator
- [JSON for Modern C++](https://github.com/nlohmann/json) - JSON library for C++

## Security

- Database is protected against SQL injection through prepared statements
- Input validation for all API endpoints
- Thread-safe operations using mutex locks
- No sensitive data exposure in error messages

