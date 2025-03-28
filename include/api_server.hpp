#pragma once
#include <crow.h>
#include "interfaces/i_request_handler.hpp"
#include "interfaces/i_database_manager.hpp"

class ApiServer 
{
public:
    explicit ApiServer(const std::string& db_path);
    ApiServer(std::shared_ptr<IDatabaseManager> db_manager_impl, 
             std::shared_ptr<IRequestHandler> handler_impl);
    ~ApiServer();
    
    void start(uint16_t port);
    void stop();

private:
    void setup_routes();
    
    crow::SimpleApp app;
    std::shared_ptr<IDatabaseManager> db_manager;
    std::shared_ptr<IRequestHandler> handler;
};
