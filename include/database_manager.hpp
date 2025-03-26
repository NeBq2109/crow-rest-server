#pragma once
#include <string>
#include <vector>
#include <sqlite3.h>
#include "data.hpp"

class DatabaseManager 
{
public:
    explicit DatabaseManager(const std::string& db_path);
    ~DatabaseManager();

    std::vector<Data> get_all_data();
    Data insert_data(const std::string& name, double value);

private:
    sqlite3* db;
    void initialize_table();
};
