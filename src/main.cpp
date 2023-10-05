#include <chrono>
#include <cassert>
#include <iostream>

#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/types.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

#include "../include/DatabaseHandler.h"

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::basic::make_array;
using bsoncxx::builder::basic::make_document;

struct delete_response {
        int response;
        std::string id;
};

std::string get_uri()
{

        std::string username, password, url;
        std::cout << "Please enter your mongodb server url: ";
        std::cin >> url;
        std::cout << "Please enter your username: ";
        std::cin >> username;
        std::cout << "Please enter your mongodb password: ";
        std::cin >> password;
        return "mongodb+srv://" + username + ":" + password + "@" + url + "/?retryWrites=true&w=majority";

}

std::string get_connection_string()
{

        std::string connectionStr;
        std::cout << "Please enter your mongodb connection string ";
        std::cin >> connectionStr;
        return connectionStr;

}

std::string get_method()
{

        std::string method;

        std::cout << "What method would you like to use? ";
        std::cin >> method;

        return method;
}

std::string handle_get(DatabaseHandler& handler)
{

        std::string id, field;
        std::cout << "Please enter the ID of the object you would like to get: ";
        std::cin.ignore();
        std::getline(std::cin, id);
        std::cout << "Please enter the field of the object you would like to get: ";
        std::getline(std::cin, field);

        return handler.get(id, field);
}

std::string handle_post(DatabaseHandler& handler)
{

        std::string json;
        std::cout << "Please enter the data you would like to enter: ";
        std::cin.ignore();
        std::getline(std::cin, json);

        return handler.post(json);
}

int handle_put(DatabaseHandler& handler)
{
                        std::string id, json;
                        std::cout << "Please enter the id of the item you would like to update: ";
                        std::cin.ignore();
                        std::getline(std::cin, id);
                        std::cout << "Please enter the data you would like to replace it with: ";
                        std::getline(std::cin, json);

                        return handler.put(id, json);
}

delete_response handle_delete(DatabaseHandler& handler)
{

        std::string id;
        std::cout << "Please enter the ID of the object you would like to delete: ";
        std::cin >> id;
        return { handler.delete_id(id), id };

}

int main()
{
        DatabaseHandler handler { get_connection_string() };

        for (;;) {

                auto method = get_method();

                if (method == "PUT") {

                        auto response = handle_put(handler);
                        std::cout << "Response: " << response << "\n";

                } else if (method == "POST") {

                        auto response = handle_post(handler);
                        std::cout << "The ID of your entry is: " << response << "\n";

                } else if (method == "GET") {

                        auto response = handle_get(handler);
                        std::cout << "The value at ID is: " << response << "\n";;

                } else if (method == "DELETE") {

                        auto [response, id] = handle_delete(handler);

                        std::cout << (response ? "Failed to delete" : "Successfully deleted ") << id << "\n";

                } else {

                        std::cout << "Please use one of the supported methods: POST, GET, PUT, DELETE\n";

                }

        }

        return 0;
}
