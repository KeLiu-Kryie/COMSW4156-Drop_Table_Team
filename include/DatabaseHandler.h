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
using namespace std;

class DatabaseHandler {

public:
        DatabaseHandler();
//Get

        //string GetDataFromId(string id);
        string get(string id, string field);

//Post

        //string AddNewDataToCollection(string jsonData);
        string post(string json);

//Delete

        //string deleteDataWithId(string id);
        int delete_id(string id);
//Update

        //int UpdateDataInCollection(string id, string jsonData);
        int put(string id, string json);

        void setupCollectionEndpoint(string uri_str, string dbName);

private:
  // Create an instance. Note there must only be one instance
        mongocxx::instance inst{};
  //Database
        mongocxx::database db;

        mongocxx::uri uri;

        mongocxx::client client;

};
