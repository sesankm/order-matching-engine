#ifndef SERVER_H
#define SERVER_H

#include "matching_engine.hpp"
#include "peer.hpp"

class Server : public Peer {
    MatchingEngine orderMatcher;

    std::string back_buffer {};
    int accept_desc {-1};

    std::mutex mut;

    void serve_conn(int desc);
    void message_processor();

public:
    Server(int, int, int, int);
    void operator()() override;
    ~Server();
};


#endif
