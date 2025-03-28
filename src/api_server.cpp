#include "api_server.hpp"

ApiServer::ApiServer(const std::string& db_path)
    : db_manager(std::make_shared<DatabaseManager>(db_path))
    , handler(std::make_shared<RequestHandler>(db_manager))
{
    setup_routes();
}

ApiServer::~ApiServer() 
{
    try
    {
        ApiServer::stop();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}

void ApiServer::setup_routes() 
{
    CROW_ROUTE(app, "/api/data")
        .methods(crow::HTTPMethod::GET)
        ([this] { return handler->handle_get_all(); });

    CROW_ROUTE(app, "/api/data")
        .methods(crow::HTTPMethod::POST)
        ([this](const crow::request& req) { return handler->handle_create(req); });
}

void ApiServer::start(uint16_t port) 
{
    app.port(port).multithreaded().run();
}

void ApiServer::stop() 
{
    app.stop();
}
