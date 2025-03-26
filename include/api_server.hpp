#pragma once
#include <crow.h>
#include <memory>
#include "database_manager.hpp"
#include "request_handler.hpp"

class ApiServer 
{
public:
    explicit ApiServer(const std::string& db_path);
    ~ApiServer();
    void start(uint16_t port = 8080);
    void stop();

private:
    crow::SimpleApp app;
    std::shared_ptr<DatabaseManager> db_manager;
    std::shared_ptr<RequestHandler> handler;
    
    void setup_routes();
};
