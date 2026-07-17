#include "client.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>

Client::Client(int addr_family, int socket_type, int flags, int port)
    : Peer { addr_family, socket_type, flags, port } {
    Peer::sock_syscall(connect, socket_desc, info->ai_addr, info->ai_addrlen);
}

void Client::operator()() {
    std::atomic<int> total_messages_sent { 0 };
    std::atomic<size_t> total_bytes_sent { 0 };
    std::string input {};

    std::atomic<int> active_threads { 0 };
    std::ifstream fstream {"../input/input.txt"};

    while (getline(fstream, input)) {
        ++active_threads;
        try {
            std::thread{
                [this, &total_bytes_sent, &total_messages_sent, &active_threads](std::string input) {
                    input.push_back('\n');
                    const size_t sent_size = send(socket_desc, input.data(), input.size(), 0);
                    total_bytes_sent += sent_size;
                    ++total_messages_sent;
                    --active_threads;
                }, input
            }.detach();
        } catch (...) {
            --active_threads;
        }
        input.clear();
    }

    while (active_threads > 0) {
        std::cout << "Waiting for active threads to complete...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    std::cout << "Closing client. "
              << "Total messages sent: " << total_messages_sent
              << ", total bytes sent: "  << total_bytes_sent << "\n";

    close(socket_desc);
}
