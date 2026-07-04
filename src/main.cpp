#include <thread>
#include "server.hpp"
#include "client.hpp"

int main()
{
    Server s { AF_INET, SOCK_STREAM, 0, 8081 };
    std::thread t1 { std::ref(s) };

    Client c    { AF_INET, SOCK_STREAM, 0, 8081 };
    Client c1   { AF_INET, SOCK_STREAM, 0, 8081 };
    Client c2   { AF_INET, SOCK_STREAM, 0, 8081 };
    std::thread { std::ref(c) }.detach();
    std::thread { std::ref(c1) }.detach();
    std::thread { std::ref(c2) }.detach();

    t1.join();
    return 0;
}
