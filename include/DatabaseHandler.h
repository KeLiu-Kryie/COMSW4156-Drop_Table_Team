#include <chrono>

#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/json.hpp>
#include <bsoncxx/types.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;

class DatabaseHandler {

public:
        DatabaseHandler();
//Get

        //std::string GetDataFromId(std::string id);
        std::string get(std::string id, std::string field);

//Post

        //std::string AddNewDataToCollection(std::string jsonData);
        std::string post(std::string json);

//Delete

        //std::string deleteDataWithId(std::string id);
        int delete_id(std::string id);
//Update

        //int UpdateDataInCollection(std::string id, std::string jsonData);
        int put(std::string id, std::string json);

        void setupCollectionEndpoint(std::string uri_str, std::string dbName);

private:
  // Create an instance. Note there must only be one instance
        mongocxx::instance inst{};
  //Database
        mongocxx::database db;

        mongocxx::uri uri;

        mongocxx::client client;

};
