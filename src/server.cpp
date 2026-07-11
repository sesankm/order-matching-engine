#include "server.hpp"
#include <iostream>
#include <thread>

Server::Server(int addr_family, int socket_type, int flags, int port)
    : Peer { addr_family, socket_type, flags, port } {
    Peer::sock_syscall(bind, socket_desc, info->ai_addr, info->ai_addrlen);
    Peer::sock_syscall(listen, socket_desc, 20);
}

void Server::serve_conn(int desc) {
    auto serve = [desc]() {
        thread_local char buffer[BUFF_SIZE];
        while (int buff_size = recv(desc, buffer, BUFF_SIZE, 0)) {
            if (buff_size > 0) {
              std::cout << desc
                        << "> RECEIVED DATA: "
                        << buffer << ". Size recieved: " << buff_size << "\n";
                memset(buffer, 0, BUFF_SIZE);
                int ret = send(desc, "200", 3, 0);
                if (ret < 0) {
                    throw std::runtime_error(strerror(errno));
                }
                std::cout << desc << "> Served\n\n";
            } else {
                break;
            }
        }
        std::cout << "Closing connection: " + std::to_string(desc) << "\n\n";
        close(desc);
    };

    std::thread t1 { serve };
    t1.detach();
}

void Server::operator()() {
    struct sockaddr client_addr {};
    socklen_t client_len { 0 };
    int counter {0};

    while(1) {
        accept_desc = accept(socket_desc, &client_addr, &client_len);
        std::cout << "** Accepted connection: " << accept_desc << "\n\n";
        if (accept_desc < 0) { throw std::runtime_error(strerror(errno)); }
        serve_conn(accept_desc);
    }
}


Server::~Server() {
    if (accept_desc >= 0) { close(accept_desc); }
}
