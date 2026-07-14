#include "server.hpp"
#include <iostream>
#include <thread>
#include "message_parser.hpp"

Server::Server(int addr_family, int socket_type, int flags, int port)
    : Peer { addr_family, socket_type, flags, port } {
    sock_syscall(bind, socket_desc, info->ai_addr, info->ai_addrlen);
    sock_syscall(listen, socket_desc, 20);
}

const char* CONF_MSG = "Order placed.";
const char* ERR_MSG  = "Unable to place order.";

void Server::serve_conn(int desc) {
    auto serve = [desc, this]() {
        thread_local char buffer[BUFF_SIZE];
        while (int buff_size = recv(desc, buffer, BUFF_SIZE, 0)) {
            if (buff_size > 0) {
              std::cout << desc
                        << "> RECEIVED DATA: "
                        << buffer << ". Size recieved: " << buff_size << "\n";
                try {
                    Order o = MessageParser::parse(buffer);
                    orderMatcher.submitOrder(std::move(o));
                    memset(buffer, 0, BUFF_SIZE);
                    send(desc, CONF_MSG, strlen(CONF_MSG), 0);
                } catch (std::runtime_error& e) {
                    send(desc, ERR_MSG, strlen(ERR_MSG), 0);
                }
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
