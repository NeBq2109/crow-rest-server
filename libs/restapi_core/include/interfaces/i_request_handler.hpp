#pragma once
#include <crow.h>

class IRequestHandler 
{
public:
    virtual ~IRequestHandler() = default;
    virtual crow::response handle_get_all() = 0;
    virtual crow::response handle_create(const crow::request& req) = 0;
};
