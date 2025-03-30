#include "database_manager.hpp"
#include "data.hpp"
#include <stdexcept>

DatabaseManager::DatabaseManager(const std::string& db_path) 
{
    if (sqlite3_open(db_path.c_str(), &db) != SQLITE_OK)
        throw std::runtime_error("Failed to open database");

    initialize_table();
}

DatabaseManager::~DatabaseManager() 
{
    if (db)
        sqlite3_close(db);
}

std::vector<Data> DatabaseManager::get_all_data() 
{
    std::vector<Data> results;
    const char* sql = "SELECT id, name, value FROM data;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        throw std::runtime_error("Failed to prepare statement");

    while (sqlite3_step(stmt) == SQLITE_ROW) 
    {
        Data item
        {
            sqlite3_column_int(stmt, 0),
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
            sqlite3_column_double(stmt, 2)
        };
        results.push_back(item);
    }

    sqlite3_finalize(stmt);
    return results;
}

Data DatabaseManager::insert_data(const std::string& name, double value) 
{
    const char* sql = "INSERT INTO data (name, value) VALUES (?, ?) RETURNING id;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
        throw std::runtime_error("Failed to prepare statement");

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 2, value);

    int new_id = -1;
    if (sqlite3_step(stmt) == SQLITE_ROW)
        new_id = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);

    if (new_id == -1)
        throw std::runtime_error("Failed to insert data");

    return Data{new_id, name, value};
}

void DatabaseManager::initialize_table() 
{
    const char* sql = R"(
        CREATE TABLE IF NOT EXISTS data (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            value REAL NOT NULL
        );
    )";

    char* err_msg = nullptr;
    if (sqlite3_exec(db, sql, nullptr, nullptr, &err_msg) != SQLITE_OK) 
    {
        std::string error = err_msg;
        sqlite3_free(err_msg);
        throw std::runtime_error("Failed to create table: " + error);
    }
}