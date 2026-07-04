#ifndef SERVER_H
#define SERVER_H

#include "peer.hpp"

class Server : public Peer {
    int accept_desc {-1};

public:
    Server(int, int, int, int);
    void operator()() override;
    ~Server();
};


#endif
