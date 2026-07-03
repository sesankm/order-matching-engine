#include <netdb.h>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>

static constexpr int BUFF_SIZE { 100 };

template <typename ... T>
void sock_syscall(auto* fp, T ... args) {
    // returns zero on success
    if (fp(args...)) { throw std::runtime_error(strerror(errno)); }
}

class Server {
    int socket_desc {-1};
    int client_desc {-1};
    struct addrinfo* info;
    char* buffer {};

public:
    Server(int addr_family, int socket_type, int flags, int port) {
        buffer = (char*)malloc(BUFF_SIZE);
        socket_desc = socket(PF_INET, SOCK_STREAM, 0);
        struct addrinfo hints = {
            .ai_flags = flags,
            .ai_family = addr_family,
            .ai_socktype = socket_type
        };
        sock_syscall(getaddrinfo, nullptr, std::to_string(port).c_str(), &hints, &info);
        sock_syscall(bind, socket_desc, info->ai_addr, info->ai_addrlen);
        sock_syscall(listen, socket_desc, 10);
    }

    void serve() {
        struct sockaddr client_addr;
        socklen_t client_len;

        while(1) {
            client_desc = accept(socket_desc, &client_addr, &client_len);
            if (client_desc < 0) { throw std::runtime_error(strerror(errno)); }
            int buff_size = recv(client_desc, buffer, BUFF_SIZE, 0);
            if (buff_size > 0) {
                std::cout << buffer << "\n";
                memset(buffer, 0, buff_size);

                int ret = send(client_desc, "200", 3, 0);
                if (ret < 0) { throw std::runtime_error(strerror(errno)); }
                break;
            }
        }
    }

    ~Server() {
        close(client_desc);
        close(socket_desc);
        free(buffer);
    }
};

int main()
{
    Server s { AF_INET, SOCK_STREAM, 0, 8080 };
    s.serve();
    return 0;
}
