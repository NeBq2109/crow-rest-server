# Crow REST Server Example

A simple REST API server example built with [Crow C++ Framework](https://crowcpp.org).

## Prerequisites

- C++17 compatible compiler
- CMake 3.14 or higher
- Boost library
- Git

## Building the Project

```bash
# Clone the repository
git clone https://github.com/NeBq2109/crow-rest-server.git
cd crow-rest-server

# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
make
```

## Running the Server

From the build directory:
```bash
./crow_rest_api
```

The server will start on `http://localhost:8080` by default.

## API Endpoints

### GET /
Returns a welcome message to verify the server is running.

```bash
curl http://localhost:8080/
```

### Health Check
Open your browser and navigate to:
```
http://localhost:8080/
```

You should see a welcome message confirming the server is running properly.

## Project Structure

```
crow-rest-server/
├── CMakeLists.txt
├── README.md
└── src/
    └── main.cpp
```

## Contributing

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

