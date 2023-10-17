#include "../include/DatabaseHandler.h"

#include <typeinfo>

DatabaseHandler::DatabaseHandler(){}

std::string DatabaseHandler::get(std::string id, std::string field) {

        try {

                bsoncxx::oid currId = bsoncxx::oid(id);
                
                auto collection = db["translations"];
                auto doc = collection.find_one(make_document(kvp("_id", currId)));

                if (!doc)
                        return "nothing";

                auto view = doc->view();
                auto element = view[field];

                auto value = element.get_string().value;

                return std::string(value);

        } catch (const std::exception& e) {

                std::cerr << "Exception: " << e.what() << std::endl;

        }

        return "ERROR";
}

//int DatabaseHandler::UpdateDataInCollection(std::string id, std::string jsonData){
int DatabaseHandler::put(std::string id, std::string json)
{

        try {
                bsoncxx::oid currId = bsoncxx::oid(id);
                auto response = db["translations"].update_one(make_document(kvp("_id", currId)), make_document(kvp("$set", make_document(kvp("userData", json)))));
                if(response->modified_count() > 0){
                        return 200;
                } else {
                        return 404;
                }
        } catch (const std::exception& e) {
                std::cerr<< "Exception: " << e.what() << std::endl;
                return 500;
        }

        return 500;
}

//std::string DatabaseHandler::AddNewDataToCollection(std::string jsonData){
std::string DatabaseHandler::post(std::string json)
{

        try {
                bsoncxx::document::value new_doc = make_document(kvp("userData", json));
                auto res = db["translations"].insert_one(std::move(new_doc));
                bsoncxx::oid oid = res->inserted_id().get_oid().value;
                return oid.to_string();
        } catch (const std::exception& e) {
                std::cerr<< "Exception: " << e.what() << std::endl;
                return e.what();
        }

        return "ERROR";
}

int DatabaseHandler::delete_id(std::string id)
{
        try {

                auto collection = db["translations"];

                auto deletion = collection.delete_one(make_document(kvp("_id", bsoncxx::oid(id))));

                if (!deletion || deletion->deleted_count() != 1)
                        return 1;

                return 0;

        } catch (const std::exception& e) {

                std::cerr<< "Exception: " << e.what() << std::endl;
                return 1;

        }

        return 1;
}

void DatabaseHandler::setupCollectionEndpoint(std::string uri_str, std::string dbName){
        try {
                // Replace the connection std::string with your MongoDB deployment's connection std::string.
                uri = mongocxx::uri(uri_str);
                client = mongocxx::client(uri);
                db = client[dbName];
        } catch (const std::exception& e) {
                // Handle errors.
                std::cerr<< "Exception: " << e.what() << std::endl;
        }
}
