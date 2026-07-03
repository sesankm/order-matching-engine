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
        sock_syscall(listen, socket_desc, 10);
    }

    void serve() {
        struct sockaddr client_addr {};
        socklen_t client_len { 0 };
        std::vector<char> buffer(BUFF_SIZE, '\0');
        while(1) {
            client_desc = accept(socket_desc, &client_addr, &client_len);
            if (client_desc < 0) { throw std::runtime_error(strerror(errno)); }
            int buff_size = recv(client_desc, buffer.data(), BUFF_SIZE, 0);
            if (buff_size > 0) {
                std::cout << std::string_view(buffer.data(), buff_size) << "\n";
                int ret = send(client_desc, "200", 3, 0);
                if (ret < 0) { throw std::runtime_error(strerror(errno)); }
                // TODO: Continuously serve requests
                break;
            }
        }
    }

    ~Server() {
        if (client_desc >= 0) { close(client_desc); }
        if (socket_desc >= 0) { close(socket_desc); }
    }
};

int main()
{
    Server s { AF_INET, SOCK_STREAM, 0, 8080 };
    s.serve();
    return 0;
}
