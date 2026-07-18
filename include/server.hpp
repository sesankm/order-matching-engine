#ifndef SERVER_H
#define SERVER_H

#include "matching_engine.hpp"
#include "peer.hpp"

class Server : public Peer {
    MatchingEngine orderMatcher;

    std::string shared_buffer {};
    std::condition_variable cond_var;
    std::mutex write_mut;

    void msg_reader(int desc);
    void msg_processor();

public:
    Server(int, int, int, int);
    void operator()() override;
    ~Server();
};


#endif
