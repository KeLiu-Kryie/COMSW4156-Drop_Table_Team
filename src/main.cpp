#include <iostream>

#include "../include/Server.h"

int main()
{
    Server serviceHost;
    serviceHost.run(18080);
    return 0;
}
