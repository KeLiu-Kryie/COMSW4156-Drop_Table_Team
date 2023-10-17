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
//Get transaltion history

        std::pair<int, string> get_translation_history(string id);

//Post

        std::pair<int, string> create_user();

//Delete

        std::pair<int, string> delete_user(string id);

        std::pair<int, string> post_translation(string id, string translationDataJson);

        void setupCollectionEndpoint(string uri_str, string dbName);

private:
  // Create an instance. Note there must only be one instance
        mongocxx::instance inst{};
  //Database
        mongocxx::database db;

        mongocxx::uri uri;

        mongocxx::client client;

};
