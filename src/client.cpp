#include "client.hpp"

Client::Client(int addr_family, int socket_type, int flags, int port)
    : Peer { addr_family, socket_type, flags, port } {
    Peer::sock_syscall(connect, socket_desc, info->ai_addr, info->ai_addrlen);
}

void Client::operator()() {
    for (int i = 1; i < 5; i++) {
        snprintf(buffer, BUFF_SIZE, "Iterator: %d", i); // sprintf deprecated on clang, wtf
        send(socket_desc, buffer, BUFF_SIZE, 0);
    }
    close(socket_desc);
}
