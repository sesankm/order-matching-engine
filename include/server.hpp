#ifndef SERVER_H
#define SERVER_H

#include "matching_engine.hpp"
#include "peer.hpp"
#include "ring_buffer.hpp"

class Server : public Peer {
    MatchingEngine orderMatcher;
    RingBuffer ringBuffer;
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
