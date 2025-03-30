#pragma once
#include <shared_mutex>
#include "interfaces/i_database_manager.hpp"
#include "interfaces/i_request_handler.hpp"

class RequestHandler final : public IRequestHandler
{
public:
    explicit RequestHandler(std::shared_ptr<IDatabaseManager> db);
    crow::response handle_get_all() override;
    crow::response handle_create(const crow::request& req) override;

private:
    std::shared_ptr<IDatabaseManager> db_manager;
    std::shared_mutex db_mutex;
};
