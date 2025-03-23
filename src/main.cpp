#include <crow.h>
#include <iostream>
#include <string>
#include <vector>
#include <shared_mutex>
#include <atomic>

struct Data {
    int id;
    std::string name;
    double value;
};

std::vector<Data> database = {
    {1, "Example data", 42.5}
};

std::shared_mutex db_mutex;
std::atomic<int> id_counter(2);  // Atomic id generator

// Helper function to serialize Data
crow::json::wvalue serialize_data(const Data& item) {
    crow::json::wvalue json_item;
    json_item["id"] = item.id;
    json_item["name"] = item.name;
    json_item["value"] = item.value;
    return json_item;
}

int main()
{
    crow::SimpleApp app;

    // Endpoint to get all data
    CROW_ROUTE(app, "/api/data")
    .methods(crow::HTTPMethod::GET)
    ([] {
        crow::json::wvalue result;
        result["status"] = "success";
        
        std::vector<crow::json::wvalue> data_json;

        std::shared_lock lock(db_mutex);
        for (const auto& item : database) {
            data_json.push_back(serialize_data(item));  // Serialize data
        }

        result["data"] = std::move(data_json);
        return crow::response(result);
    });

    // Endpoint to post new data
    CROW_ROUTE(app, "/api/data")
    .methods(crow::HTTPMethod::POST)
    ([](const crow::request& req) {
        auto data = crow::json::load(req.body);
        if (!data || !data.has("name") || !data.has("value")) {
            crow::json::wvalue error_response;
            error_response["status"] = "error";
            error_response["message"] = "Invalid JSON format or missing fields";
            return crow::response(400, error_response);
        }

        // Validate 'value' field
        if (data["value"].t() != crow::json::type::Number) {
            crow::json::wvalue error_response;
            error_response["status"] = "error";
            error_response["message"] = "'value' must be a number";
            return crow::response(400, error_response);
        }

        // Add the new data to the database
        std::lock_guard lock(db_mutex);
        int new_id = id_counter.fetch_add(1);  // Atomically increment the ID
        database.push_back({new_id, data["name"].s(), data["value"].d()});

        crow::json::wvalue result;
        result["status"] = "created";
        result["data"] = serialize_data(database.back());  // Serialize the newly created data
        return crow::response(201, result);
    });

    app.port(8080).multithreaded().run();
}
