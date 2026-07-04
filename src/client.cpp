#include "client.hpp"
#include <iostream>
#include <thread>

Client::Client(int addr_family, int socket_type, int flags, int port)
    : Peer { addr_family, socket_type, flags, port } {
    buffer = (char*)malloc(sizeof(BUFF_SIZE));
    Peer::sock_syscall(connect, socket_desc, info->ai_addr, info->ai_addrlen);
}

void Client::operator()() {
    std::atomic<int> total_messages_sent { 0 };
    std::atomic<size_t> total_bytes_sent { 0 };
    std::atomic<size_t> total_bytes_recv { 0 };
    size_t send_size { 0 };
    std::string input {};

    do {
        std::cout << ">> ";
        std::getline(std::cin, input);
        if (input.size() <= 0) {
            break;
        }

        std::thread {
            [this, input, &total_bytes_recv, &total_bytes_sent, &total_messages_sent]() {
                char buffer[BUFF_SIZE];
                size_t sent_size = send(socket_desc, input.data(), input.size(), 0);
                size_t recv_size = recv(socket_desc, buffer, BUFF_SIZE, 0);
                total_bytes_recv += recv_size;
                total_bytes_sent += sent_size;
                ++total_messages_sent;
            }
        }.detach();
    } while(input.size());

    std::cout << "Closing client. "
              << "Total messages sent: "    << total_messages_sent
              << ", total bytes sent: "     << total_bytes_sent
              << ", total bytes recieved: " << total_bytes_recv << "\n";

    close(socket_desc);
}
