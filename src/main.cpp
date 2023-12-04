#include <iostream>

#include "../include/Server.h"

int main()
{
    Server serviceHost;
    serviceHost.run(8080);
    return 0;
}
