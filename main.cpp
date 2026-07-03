#include <netdb.h>
#include <stdexcept>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

template <typename ... T>
void sock_syscall(auto* fp, T ... args) {
    if (fp(args...) < 0)  {
        throw std::runtime_error(strerror(errno));
    }
}

class Server {
    int socket_desc;
    struct addrinfo* info;

public:
    Server(int addr_family, int socket_type, int flags, int port) {
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

    void kill_conn(int descriptor);

    ~Server() {
        close(socket_desc);
    }
};

int main()
{
    Server s { AF_INET, SOCK_STREAM, 0, 8080 };
    return 0;
}
