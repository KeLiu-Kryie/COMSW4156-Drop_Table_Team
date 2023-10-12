#include <iostream>
#include "../include/User.h"

#include "../include/Server.h"

using json = nlohmann::json;

int main()
{
    // Server serviceHost;
    // serviceHost.run();
    //Serialize to json
    User p = User("Flanders", "Priest");
    json j = p;
    std::cout<<j<<std::endl;
    //Deserialize json to class
    std::string jSt = j.dump();
    User p2 = json::parse(jSt);
    // auto p2 = j.template get<User>();
    return 0;
}
