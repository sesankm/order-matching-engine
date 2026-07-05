# socket

## listening side

- `socket()` - Kernel creates a SocketControlBlock struct for listening socket.
- `bind()` - Kernel attaches port number/addr to SocketControlBlock
- `listen()` - Creates SYN and Accept queues.
  - Whenever kernel recieves a SYN packet, it checks list of listening SocketControlBlocks.
  - If a struct exists for the combination of port/addr in SYN, kernel sends back a SYNACK.
  - Kernel also adds a request_sock entry into the SYN queue.
  - When kernel recieves the final ACK packet, it promotes request_sock from SYN queue to a full SocketControlBlock in the accept queue. (deleted from SYN queue, added to accept queue)
  - *this configures the listening socket*
- `accept()` - this waits for active connections.
  - when client connects, kernel peels off an entry from accept queue and returns descriptor to userspace
  - if nothing is in the accept queue, this process sleeps and gets woken up by kernel when something enters accept queue
- `close(fd)` - closes the SocketControlBlock for given descriptor.
  - listening side has one listening socket, and one socket per active connection.

## connecting side

- `socket()` - Kernel creates an empty SocketControlBlock. Unlike listening side, this is just an empty struct that does nothing.
- `connect()` - initiates the 3 way handshake.
  - after sending the SYN, kernel fills in the empty SocketControlBlock from previous step
    - dest port/addr
    - kernel assigned local port/addr
    - state gets updated from TCP_CLOSE to TCP_SYN_SENT
    - after 3 way handshake completes, state gets set to TCP_ESTABLISHED
    - this is all in kernel space
  - Major difference here compared to listening side is that we only have one struct here. Listening side has one socket/struct to listen for all incoming connections and an addition socket/struct *per* active connection.