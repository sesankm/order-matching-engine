#include <netdb.h>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

#include "client.cpp"

static constexpr int BUFF_SIZE { 2400 };

// template <typename ... T>
// void sock_syscall(auto* fp, T ... args) {
//     // returns zero on success
//     if (fp(args...)) { throw std::runtime_error(strerror(errno)); }
// }

class Server {
    int socket_desc {-1};
    int accept_desc {-1};
    struct addrinfo* info;

public:
    Server(int addr_family, int socket_type, int flags, int port) {
        struct addrinfo hints = {
            .ai_flags = flags,
            .ai_family = addr_family,
            .ai_socktype = socket_type
        };
        socket_desc = socket(hints.ai_family, hints.ai_socktype, flags);
        sock_syscall(getaddrinfo, nullptr, std::to_string(port).c_str(), &hints, &info);
        sock_syscall(bind, socket_desc, info->ai_addr, info->ai_addrlen);
        sock_syscall(listen, socket_desc, 20);
    }

    void operator() () {
        serve();
    }

    void serve() {
        struct sockaddr client_addr {};
        socklen_t client_len { 0 };
        std::vector<char> buffer(BUFF_SIZE, '\0');

        int counter {0};

        while(1) {
            accept_desc = accept(socket_desc, &client_addr, &client_len);
            std::cout << "Accepted\n";
            if (accept_desc < 0) { throw std::runtime_error(strerror(errno)); }
            while (int buff_size = recv(accept_desc, buffer.data(), BUFF_SIZE, 0)) {
                if (buff_size > 0) {
                    std::cout << std::string_view(buffer.data(), buff_size) << "\n";
                    int ret = send(accept_desc, "200", 3, 0);
                    if (ret < 0) { throw std::runtime_error(strerror(errno)); }
                    std::cout << "Served\n\n\n";
                    if (++counter > 13) {
                        break;
                    }
                } else {
                    std::cout << "Closing connection: " + std::to_string(accept_desc) << "\n";
                    close(accept_desc);
                    break;
                }
            }
        }
    }

    ~Server() {
        if (accept_desc >= 0) { close(accept_desc); }
        if (socket_desc >= 0) { close(socket_desc); }
    }
};

int main()
{
    Server s { AF_INET, SOCK_STREAM, 0, 8081 };

    Client c { AF_INET, SOCK_STREAM, 0, 8081 };
    Client c2 { AF_INET, SOCK_STREAM, 0, 8081 };
    Client c3 { AF_INET, SOCK_STREAM, 0, 8081 };

    std::thread t1 { std::ref(s) };
    std::thread t2 { std::ref(c) };
    std::thread t3 { std::ref(c2) };
    std::thread t4 { std::ref(c3) };

    t2.detach();
    t3.detach();
    t4.detach();

    t1.join();

    return 0;
}
