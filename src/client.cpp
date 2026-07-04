#include "client.hpp"

Client::Client(int addr_family, int socket_type, int flags, int port)
    : Peer { addr_family, socket_type, flags, port } {
    buffer = (char*)malloc(sizeof(BUFF_SIZE));
    Peer::sock_syscall(connect, socket_desc, info->ai_addr, info->ai_addrlen);
}

void Client::operator()() {
    size_t total_bytes_sent { 0 };
    size_t total_bytes_recv { 0 };
    size_t send_size { 0 };
    size_t recv_size { 0 };
    for (int i = 1; i < 5; i++) {
        snprintf(buffer, BUFF_SIZE, "Iterator: %d", i); // sprintf deprecated on clang, wtf
        send_size = strlen(buffer);
        total_bytes_sent += send_size;
        send(socket_desc, buffer, send_size, 0);
        recv_size = recv(socket_desc, buffer, BUFF_SIZE, 0);
        total_bytes_recv += recv_size;
        printf("-- %d> Recieved from server: %.*s -- \n\n", socket_desc, static_cast<int>(recv_size), buffer);
    }

    printf("*** Closing client %d. Total sent: %zu, Total recv: %zu\n\n", socket_desc, total_bytes_sent, total_bytes_recv);

    close(socket_desc);
}
