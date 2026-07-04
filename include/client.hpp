#ifndef CLIENT_H
#define CLIENT_H

#include "peer.hpp"

class Client : public Peer {
    char* buffer;

public:
    Client(int, int, int, int);
    void operator()() override;
};


#endif
