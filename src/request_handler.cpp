#include "request_handler.hpp"
#include "utils.hpp"

RequestHandler::RequestHandler(std::shared_ptr<DatabaseManager> db)
    : db_manager(std::move(db))
{}

crow::response RequestHandler::handle_get_all() 
{
    try 
    {
        std::shared_lock lock(db_mutex);
        auto items = db_manager->get_all_data();
        
        std::vector<crow::json::wvalue> data_json;
        for (const auto& item : items)
            data_json.push_back(utils::serialize_data(item));
        
        return crow::response(utils::create_success_response(std::move(data_json)));
    } 
    catch (const std::exception& e) 
    {
        return crow::response(500, utils::create_error_response(e.what()));
    }
}

crow::response RequestHandler::handle_create(const crow::request& req) 
{
    try 
    {
        auto data = crow::json::load(req.body);
        
        if (!data || !data.has("name") || !data.has("value"))
            return crow::response(400, utils::create_error_response("Invalid JSON format or missing fields"));

        if (data["value"].t() != crow::json::type::Number)
            return crow::response(400, utils::create_error_response("'value' must be a number"));

        std::lock_guard lock(db_mutex);
        auto new_item = db_manager->insert_data(data["name"].s(), data["value"].d());
        auto json_item = utils::serialize_data(new_item);
        auto response = utils::create_success_response(std::move(json_item));
        return crow::response(201, response);
    } 
    catch (const std::exception& e) 
    {
        return crow::response(500, utils::create_error_response(e.what()));
    }
}
