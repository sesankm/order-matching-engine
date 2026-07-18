#include "server.hpp"
#include <iostream>
#include <thread>
#include "message_parser.hpp"

Server::Server(int addr_family, int socket_type, int flags, int port)
    : Peer { addr_family, socket_type, flags, port } {
    sock_syscall(bind, socket_desc, info->ai_addr, info->ai_addrlen);
    sock_syscall(listen, socket_desc, 20);
}

void Server::msg_processor() {
    while (1) {
        while (auto it = shared_buffer.find('\n')) {
            if (it == std::string::npos) {
                std::unique_lock ul{write_mut};
                cond_var.wait(ul);
                continue;
            }
            if (shared_buffer.size() > 1) { ++it; }
            std::lock_guard l { write_mut };
            Order o = MessageParser::parse(shared_buffer.substr(0, it));
            orderMatcher.submitOrder(std::move(o));
            shared_buffer.erase(0, it);
            orderMatcher.lob.dump();
        }
    }
}

void Server::msg_reader(int desc) {
    if (desc < 0) { throw std::runtime_error(strerror(errno)); }
    std::cout << "** Accepted connection: " << desc << "\n\n";
    conn_descs.push_back(desc);
    auto serve = [desc, this] {
        thread_local char buffer[BUFF_SIZE];
        while (int buff_size = recv(desc, buffer, BUFF_SIZE, 0)) {
            if (buff_size > 0) {
                std::lock_guard l {write_mut};
                cond_var.notify_one();
                /* Message size is currently small enough to where each recv is pretty much guaranteed to return a whole line.
                 * More threads, and bigger of message sizes increases the chance of messages between different
                 * threads getting interleaved, which would corrupt the shared_buffer.
                 */
                shared_buffer.append(buffer, buff_size);
                memset(buffer, 0, BUFF_SIZE);
            } else {
                break;
            }
        }
        std::cout << "Closing connection: " + std::to_string(desc) << "\n\n";
        close(desc);
        if (const auto it = std::ranges::find(conn_descs, desc);
            it != conn_descs.end()) {
            conn_descs.erase(it);
        }
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
    for (const int cd: conn_descs) { close(cd); }
}