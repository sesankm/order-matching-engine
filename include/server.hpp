#ifndef SERVER_H
#define SERVER_H

#include "peer.hpp"

class Server : public Peer {
    int accept_desc {-1};

public:
    Server(int, int, int, int);
    void operator()() override;
    void serve_conn(int desc);
    ~Server();
};


#endif
