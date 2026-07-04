#ifndef PEER_H
#define PEER_H

#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>
#include <stdexcept>
#include <string>

static const int BUFF_SIZE { 2400 };

class Peer {
public:
    Peer(int, int, int, int);
    virtual void operator() () = 0;
    ~Peer();

protected:
    int socket_desc {-1};
    struct addrinfo* info;

    template <typename ... T> void sock_syscall(auto*, T...);
};

template <typename ... T>
void Peer::sock_syscall(auto* fp, T ... args) {
    if (fp(args...)) { throw std::runtime_error(strerror(errno)); }
}

#endif
