#include <thread>
#include "client.hpp"

int main()
{
    Client c    { AF_INET, SOCK_STREAM, 0, 8081 };
    std::thread { std::ref(c) }.join();
    return 0;
}
