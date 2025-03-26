#pragma once
#include <crow.h>
#include <shared_mutex>
#include "database_manager.hpp"

class RequestHandler 
{
public:
    explicit RequestHandler(std::shared_ptr<DatabaseManager> db);
    ~RequestHandler() = default;
    crow::response handle_get_all();
    crow::response handle_create(const crow::request& req);

private:
    std::shared_ptr<DatabaseManager> db_manager;
    std::shared_mutex db_mutex;
};
