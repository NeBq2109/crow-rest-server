#include "utils.hpp"

namespace utils 
{
    crow::json::wvalue serialize_data(const Data& item) 
    {
        crow::json::wvalue json_item;
        json_item["id"] = item.id;
        json_item["name"] = item.name;
        json_item["value"] = item.value;
        return json_item;
    }

    crow::json::wvalue create_error_response(const std::string& message) 
    {
        crow::json::wvalue response;
        response["status"] = "error";
        response["message"] = message;
        return response;
    }
}
