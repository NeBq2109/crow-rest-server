#pragma once
#include <string>
#include <vector>
#include "data.hpp"

namespace rst::core
{
    class IDatabaseManager 
    {
    public:
        virtual ~IDatabaseManager() = default;
        virtual std::vector<Data> get_all_data() = 0;
        virtual Data insert_data(const std::string& name, double value) = 0;
    };
} // namespace rst::core
