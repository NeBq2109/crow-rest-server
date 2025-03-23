# Crow REST API Example

A modern C++ REST API server demonstrating the use of the [Crow Framework](https://crowcpp.org). This example implements a simple data management API with thread-safe operations and JSON responses.

## Features

- RESTful API endpoints for data management
- Thread-safe operations using `std::shared_mutex`
- JSON request/response handling
- Atomic ID generation for data entries
- Conan package management
- CMake build system

## Prerequisites

- C++17 compatible compiler
- CMake 3.14 or higher
- Conan package manager 2.x
- Git

## Building the Project

### Using the Build Script (Recommended)

The easiest way to build the project is using the provided build script:

```bash
# Clone the repository
git clone https://github.com/NeBq2109/crow-rest-server.git
cd crow-rest-server

# Make the build script executable
chmod +x build_all.sh

# Run the build script
./build_all.sh
```

The script will:
1. Set up the Conan dependencies
2. Configure CMake with the correct toolchain
3. Build the project
4. Install the executable to `build/local_install/bin`

### Manual Build

If you prefer to build manually:

```bash
# Create build directory
mkdir -p build/conan && cd build/conan

# Install dependencies
conan install ../.. --build=missing -of=.

# Configure and build
cd ..
cmake .. -DCMAKE_TOOLCHAIN_FILE=./conan/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

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
crow-rest-server/
├── CMakeLists.txt      # CMake build configuration
├── conanfile.txt       # Conan dependencies
├── build_all.sh        # Build automation script
├── README.md
└── src/
    └── main.cpp        # Server implementation
```

## Implementation Details

- Uses `std::shared_mutex` for thread-safe database operations
- Implements atomic ID generation for new records
- Validates JSON input for POST requests
- Returns appropriate HTTP status codes (200, 201, 400)
- Supports multithreaded request handling

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
- [Boost](https://www.boost.org/) - C++ libraries
- [Conan](https://conan.io/) - C/C++ package manager

