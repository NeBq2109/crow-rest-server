#pragma once
#include <crow.h>

namespace rst::core
{
    class IRequestHandler 
    {
    public:
        virtual ~IRequestHandler() = default;
        virtual crow::response handle_get_all() = 0;
        virtual crow::response handle_create(const crow::request& req) = 0;
    };
}