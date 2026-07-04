#ifndef CLIENT_H
#define CLIENT_H

#include "peer.hpp"
#include <cstdio>

class Client : public Peer {
public:
    Client(int, int, int, int);
    void operator()() override;
};


#endif
