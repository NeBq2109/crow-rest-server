#include <crow.h>
#include <iostream>
#include <string>
#include <vector>
#include <shared_mutex>
#include <atomic>
#include <sqlite3.h>
#include <filesystem>

struct Data {
    int id;
    std::string name;
    double value;
};

class DatabaseManager {
public:
    DatabaseManager(const std::string& db_path) {
        if (sqlite3_open(db_path.c_str(), &db) != SQLITE_OK) {
            throw std::runtime_error("Failed to open database");
        }
        initialize_table();
    }

    ~DatabaseManager() {
        if (db) {
            sqlite3_close(db);
        }
    }

    std::vector<Data> get_all_data() {
        std::vector<Data> results;
        const char* sql = "SELECT id, name, value FROM data;";
        
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error("Failed to prepare statement");
        }

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Data item{
                sqlite3_column_int(stmt, 0),
                reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)),
                sqlite3_column_double(stmt, 2)
            };
            results.push_back(item);
        }

        sqlite3_finalize(stmt);
        return results;
    }

    Data insert_data(const std::string& name, double value) {
        const char* sql = "INSERT INTO data (name, value) VALUES (?, ?) RETURNING id;";
        
        sqlite3_stmt* stmt;
        if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error("Failed to prepare statement");
        }

        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_double(stmt, 2, value);

        int new_id = -1;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            new_id = sqlite3_column_int(stmt, 0);
        }

        sqlite3_finalize(stmt);

        if (new_id == -1) {
            throw std::runtime_error("Failed to insert data");
        }

        return Data{new_id, name, value};
    }

private:
    sqlite3* db;

    void initialize_table() {
        const char* sql = R"(
            CREATE TABLE IF NOT EXISTS data (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                name TEXT NOT NULL,
                value REAL NOT NULL
            );
        )";

        char* err_msg = nullptr;
        if (sqlite3_exec(db, sql, nullptr, nullptr, &err_msg) != SQLITE_OK) {
            std::string error = err_msg;
            sqlite3_free(err_msg);
            throw std::runtime_error("Failed to create table: " + error);
        }
    }
};

// Helper function to serialize Data
crow::json::wvalue serialize_data(const Data& item) {
    crow::json::wvalue json_item;
    json_item["id"] = item.id;
    json_item["name"] = item.name;
    json_item["value"] = item.value;
    return json_item;
}

int main() {
    // Create data directory if it doesn't exist
    std::filesystem::create_directories("data");
    
    // Initialize database
    DatabaseManager db_manager("data/crow_api.db");
    std::shared_mutex db_mutex;

    crow::SimpleApp app;

    // Get all data endpoint
    CROW_ROUTE(app, "/api/data")
    .methods(crow::HTTPMethod::GET)
    ([&db_manager, &db_mutex] {
        crow::json::wvalue result;
        result["status"] = "success";
        
        std::vector<crow::json::wvalue> data_json;
        
        try {
            std::shared_lock lock(db_mutex);
            auto items = db_manager.get_all_data();
            
            for (const auto& item : items) {
                data_json.push_back(serialize_data(item));
            }
            
            result["data"] = std::move(data_json);
            return crow::response(result.dump(2) + "\n");
        } catch (const std::exception& e) {
            result["status"] = "error";
            result["message"] = e.what();
            return crow::response(500, result);
        }
    });

    // Add new data endpoint
    CROW_ROUTE(app, "/api/data")
    .methods(crow::HTTPMethod::POST)
    ([&db_manager, &db_mutex](const crow::request& req) {
        crow::json::wvalue result;
        
        try {
            auto data = crow::json::load(req.body);
            if (!data || !data.has("name") || !data.has("value")) {
                result["status"] = "error";
                result["message"] = "Invalid JSON format or missing fields";
                return crow::response(400, result);
            }

            if (data["value"].t() != crow::json::type::Number) {
                result["status"] = "error";
                result["message"] = "'value' must be a number";
                return crow::response(400, result);
            }

            std::lock_guard lock(db_mutex);
            auto new_item = db_manager.insert_data(
                data["name"].s(),
                data["value"].d()
            );

            result["status"] = "created";
            result["data"] = serialize_data(new_item);
            return crow::response(201, result);
            
        } catch (const std::exception& e) {
            result["status"] = "error";
            result["message"] = e.what();
            return crow::response(500, result);
        }
    });

    // Start the server
    app.port(8080).multithreaded().run();
}
