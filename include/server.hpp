#ifndef SERVER_H
#define SERVER_H

#include "matching_engine.hpp"
#include "peer.hpp"

class Server : public Peer {
    MatchingEngine orderMatcher;
    int accept_desc {-1};
    void serve_conn(int desc);

public:
    Server(int, int, int, int);
    void operator()() override;
    ~Server();
};


#endif
