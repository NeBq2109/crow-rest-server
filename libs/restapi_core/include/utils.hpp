#pragma once
#include <crow.h>
#include "data.hpp"

namespace utils 
{
    crow::json::wvalue serialize_data(const Data& item);
    crow::json::wvalue create_error_response(const std::string& message);
    
    template <typename T>
    std::string create_success_response(T&& data) 
    {
        crow::json::wvalue response;
        response["status"] = "success";
        response["data"] = std::move(data);
        return response.dump(2) + "\n";
    }
}
