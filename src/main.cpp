#include <iostream>

#include "../include/Server.h"

int main()
{
    int a[10];
    a[10] = 0;

    Server serviceHost;
    serviceHost.run(18080);
    return 0;
}
