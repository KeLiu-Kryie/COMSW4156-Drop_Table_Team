#include "../include/DatabaseHandler.h"

#include <fstream>
#include <typeinfo>
#include <nlohmann/json.hpp>

DatabaseHandler::DatabaseHandler(std::string configPath){
        nlohmann::json config;
        std::ifstream configFile(configPath);
        config = nlohmann::json::parse(configFile);
        std::string connectionStr = config["ConnectionString"];
        std::string dbName = config["DBName"];
        uri = mongocxx::uri(connectionStr);
        client = mongocxx::client(uri);
        db = client[dbName];
}

std::pair<int, std::string> DatabaseHandler::post_translation(std::string id, const TranslationOutput& newTranslation){
        try{
                auto collection = db["translations"];
                auto doc = collection.find_one(make_document(kvp("_id", bsoncxx::oid(id))));
                if (!doc)
                        return std::make_pair(404, "ID not found!");

                auto view = doc->view();
                auto element = view["translationData"];

                auto value = element.get_string().value;
                Translations currData = nlohmann::json::parse(value.to_string());
                currData.AddTranslation(newTranslation);
                nlohmann::json updatedJson = currData;
                std::string updatedData = updatedJson.dump();
                auto response = db["translations"].update_one(make_document(kvp("_id", bsoncxx::oid(id))), make_document(kvp("$set", make_document(kvp("translationData", updatedData)))));
                if (response->modified_count() > 0) {
                        return std::make_pair(200, "Update successful");
                } else {
                        return std::make_pair(404, "ID not found!");
                }
        }
        catch (const std::exception& e) {
                std::cerr<< "Exception: " << e.what() << std::endl;
                return std::make_pair(500, e.what());
        }

        return std::make_pair(500, "Server error!");
}

std::pair<int, std::string> DatabaseHandler::get_translation_history(std::string id) {
        try {
                auto collection = db["translations"];
                auto doc = collection.find_one(make_document(kvp("_id", bsoncxx::oid(id))));

                if (!doc)
                        return std::make_pair(404, "ID not found!");

                auto view = doc->view();
                auto element = view["translationData"];

                auto value = element.get_string().value;

                return std::make_pair(200, value.to_string());
        } catch (const std::exception& e) {
                std::cerr << "Exception: " << e.what() << std::endl;
                return std::make_pair(500, e.what());
        }
        return std::make_pair(500, "Server error!");
}

std::pair<int, std::string> DatabaseHandler::create_client()
{
        try {
                Translations translations = Translations();
                nlohmann::json newData = translations;
                bsoncxx::document::value new_doc = make_document(kvp("translationData", newData.dump()));
                auto res = db["translations"].insert_one(std::move(new_doc));
                bsoncxx::oid oid = res->inserted_id().get_oid().value;
                return std::make_pair(200, oid.to_string());
        } catch (const std::exception& e) {
                std::cerr<< "Exception: " << e.what() << std::endl;
                return std::make_pair(500, e.what());
        }
        return std::make_pair(500, "Server error!");
}

std::pair<int, std::string> DatabaseHandler::delete_client(std::string id)
{
        try {
                auto collection = db["translations"];

                auto deletion = collection.delete_one(make_document(kvp("_id", bsoncxx::oid(id))));

                if (!deletion || deletion->deleted_count() != 1)
                        return std::make_pair(404, "ID not found!");

                return std::make_pair(200, "Delete Successful");
        } catch (const std::exception& e) {
                std::cerr<< "Exception: " << e.what() << std::endl;
                return std::make_pair(500, e.what());
        }
        return std::make_pair(500, "Server error");
}
