#include "client.hpp"
#include <chrono>
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
    std::string input {};

    std::atomic<int> active_threads { 0 };

    do {
        std::cout << ">> ";
        std::getline(std::cin, input);
        if (input.size() <= 0) { break; }

        std::thread {
            [this, input, &total_bytes_recv, &total_bytes_sent, &total_messages_sent, &active_threads]() {
                ++active_threads;
                char buffer[BUFF_SIZE];
                size_t sent_size = send(socket_desc, input.data(), input.size(), 0);
                /* This is kind of a socket race condition. Current thread might read a message
                   that the server sent in response to a different thread's send call.
                   It's only used to track total data sent/recv, so it wont really cause issues. */
                size_t recv_size = recv(socket_desc, buffer, BUFF_SIZE, 0);
                total_bytes_recv += recv_size;
                total_bytes_sent += sent_size;
                ++total_messages_sent;
                --active_threads;
            }
        }.detach();

    } while(input.size());

    while (active_threads > 0) {
        std::cout << "Waiting for active threads to complete...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    std::cout << "Closing client. "
              << "Total messages sent: "    << total_messages_sent
              << ", total bytes sent: "     << total_bytes_sent
              << ", total bytes recieved: " << total_bytes_recv << "\n";

    close(socket_desc);
}
