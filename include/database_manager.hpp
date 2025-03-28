#pragma once
#include <sqlite3.h>
#include <string>
#include "interfaces/i_database_manager.hpp"

class DatabaseManager final : public IDatabaseManager
{
public:
    explicit DatabaseManager(const std::string& db_path);
    ~DatabaseManager();

    std::vector<Data> get_all_data() override;
    Data insert_data(const std::string& name, double value) override;

private:
    sqlite3* db;
    void initialize_table();
};
