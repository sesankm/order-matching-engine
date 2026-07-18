#include "server.hpp"
#include <iostream>
#include <mutex>
#include <thread>
#include "message_parser.hpp"

Server::Server(int addr_family, int socket_type, int flags, int port)
    : Peer { addr_family, socket_type, flags, port }, ringBuffer(128) {
    sock_syscall(bind, socket_desc, info->ai_addr, info->ai_addrlen);
    sock_syscall(listen, socket_desc, 20);
}

void Server::msg_processor() {
    /* TODO: Fix the realloc race condition
     * shared_buffer.append() in the reader thread could trigger a realloc, which
     * could invalidate shared_buffer leading to UB. */
    while (1) {
        std::unique_lock ul{write_mut};
        cond_var.wait(ul); 
        while(!ringBuffer.isEmpty()) {
            auto message = ringBuffer.read();
            if (message.has_value()) {
                Order o = MessageParser::parse(message.value());
                orderMatcher.submitOrder(std::move(o));
            }
        }
        orderMatcher.lob.dump();
    }
}

void Server::msg_reader(int desc) {
    const char* delim = "\n";
    if (desc < 0) { throw std::runtime_error(strerror(errno)); }
    std::cout << "** Accepted connection: " << desc << "\n\n";
    auto serve = [desc, delim, this] {
        thread_local char buffer[BUFF_SIZE];
        while (int buff_size = recv(desc, buffer, BUFF_SIZE, 0)) {
            if (buff_size <= 0) { break; }
            char* token = strtok(buffer, delim);
            while(token != nullptr) {
                ringBuffer.write(token);
                cond_var.notify_one();
                token = strtok(nullptr, delim);
            }
            memset(buffer, 0, BUFF_SIZE);
        }
        std::cout << "Closing connection: " + std::to_string(desc) << "\n\n";
        close(desc);
    };

    std::thread t1 { serve };
    t1.detach();
}

void Server::operator()() {
    std::thread processor { [this] { msg_processor(); } };
    while(1) {
        const int desc = accept(socket_desc, nullptr, nullptr);
        msg_reader(desc);
    }
}

Server::~Server() {
    // First implement a way to break the infinite loop in operator()(),
    //  then update this. Currently, it's dead code.
    // for (const int cd: conn_descs) { close(cd); }
}