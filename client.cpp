#include <chrono>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string>
#include <thread>
#include <iostream>

template <typename ... T>
void sock_syscall(auto* fp, T ... args) {
    if (fp(args...)) { throw std::runtime_error(strerror(errno)); }
}

class Client {
    int socket_desc {-1};
    struct addrinfo* info;

public:
    Client(int addr_family, int socket_type, int flags, int port) {
        struct addrinfo hints = {
            .ai_flags = flags,
            .ai_family = addr_family,
            .ai_socktype = socket_type
        };
        socket_desc = socket(hints.ai_family, hints.ai_socktype, flags);
        sock_syscall(getaddrinfo, nullptr, std::to_string(port).c_str(), &hints, &info);
        sock_syscall(connect, socket_desc, info->ai_addr, info->ai_addrlen);
    }

    void operator() () {
        consume();
    }

    void consume() {
        for (int i = 1; i < 5; i++) {
            std::string to_send = "Iteration number: " + std::to_string(i) + "\n";
            std::cout << i << "\n";
            send(socket_desc, to_send.data(), 100, 0);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        close(socket_desc);
    }

    ~Client() {
        std::cout << "Client closed." << std::endl;
    }
};
