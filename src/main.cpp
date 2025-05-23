#include <filesystem>
#include "api_server.hpp"
#include <iostream>

int main() 
{
    try 
    {
        std::filesystem::create_directories("data");
        
        rst::ApiServer server("data/crow_api.db");
        server.start(8080);
    } 
    catch (const std::exception& e) 
    {
        std::cerr << "Server error: " << e.what() << '\n';
        return 1;
    }
    
    return 0;
}
