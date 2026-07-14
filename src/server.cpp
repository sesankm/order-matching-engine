#include "server.hpp"
#include <iostream>
#include <thread>
#include "message_parser.hpp"

Server::Server(int addr_family, int socket_type, int flags, int port)
    : Peer { addr_family, socket_type, flags, port } {
    sock_syscall(bind, socket_desc, info->ai_addr, info->ai_addrlen);
    sock_syscall(listen, socket_desc, 20);
}

void Server::message_processor() {
    while (1) {
        while (auto it = back_buffer.find('\n')) {
            if (it == std::string::npos) { continue; }
            if (back_buffer.size() > 1) { ++it; }
            std::lock_guard l {mut};
            std::cout << back_buffer.substr(0, it);
            Order o = MessageParser::parse(back_buffer.substr(0, it));
            orderMatcher.submitOrder(std::move(o));
            back_buffer.erase(0, it);
        }
    }
}

void Server::serve_conn(int desc) {
    auto serve = [desc, this] {
        thread_local char buffer[BUFF_SIZE];
        while (int buff_size = recv(desc, buffer, BUFF_SIZE, 0)) {
            if (buff_size > 0) {
                std::lock_guard l {mut};
                back_buffer.append(buffer, buff_size);
                memset(buffer, 0, BUFF_SIZE);
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
    sockaddr client_addr {};
    socklen_t client_len { 0 };

    //std::thread processor { [this] { message_processor(); } };
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
