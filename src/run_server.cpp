#include <thread>
#include "server.hpp"

int main()
{
    Server s { AF_INET, SOCK_STREAM, 0, 8081 };
    std::thread t1 { std::ref(s) };
    t1.join();
    return 0;
}
