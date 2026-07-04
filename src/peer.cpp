#include "peer.hpp"

Peer::Peer(int addr_family, int socket_type, int flags, int port) {
    struct addrinfo hints = {
        .ai_flags = flags,
        .ai_family = addr_family,
        .ai_socktype = socket_type
    };
    buffer = (char*)malloc(sizeof(BUFF_SIZE));
    socket_desc = socket(hints.ai_family, hints.ai_socktype, flags);
    sock_syscall(getaddrinfo, nullptr, std::to_string(port).c_str(), &hints, &info);
}

Peer::~Peer() {
    free(buffer);
    free(info);
    close(socket_desc);
}
