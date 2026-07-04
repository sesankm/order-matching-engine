#include <thread>
#include "server.hpp"
#include "client.hpp"

int main()
{
    Server s { AF_INET, SOCK_STREAM, 0, 8081 };
    Client c { AF_INET, SOCK_STREAM, 0, 8081 };

    std::thread t1 { std::ref(s) };
    std::thread t2 { std::ref(c) };

    t2.detach();
    t1.join();

    return 0;
}
