# order matching engine

An implementation of an order matching engine - the system exchanges use to match and fill buy/sell orders. This implementation uses client/server architecture with TCP sockets for order submission, a limit order book (LOB) and a matching engine with a time-price priority.

## Overview
- **Server** - accepts multiple TCP connections, each handled in it's own detached reader thread, single processor thread that feeds orders into the matching engine after reading from a shared buffer
- **Client** - reads orders from an input file and sends them over a single TCP socket, one thread per message
- **Limit Order Book (LOB)** - stores buy and sell orders. No execution logic
- **Matching engine** - sits above the LOB, detects crossed orders and fills. This is the entry point for order submissions.

# Message format
```
<SIDE> <TYPE> <PRICE> <QUANTITY>
```
- `SIDE`: `BUY` | `SELL`
- `TYPE`: `LIM` | `MKT`
- `PRICE`: decimal (converted internally to integer ticks for comparison)
- `QUANTITY`: integer

Example: `BUY LIM 10.50 100`


## Build and Run
```
cmake -B cmake-build-debug
cmake --build cmake-build-debug
```
Targets: `server`, `client`, plus test binaries (`lobtest`, `matchtest`, etc.).

```
./cmake-build-debug/server   # listens on port 8081
./cmake-build-debug/client   # sends orders from input/input.txt
```

## Todo
- Generate market depth visual. Something like TOS's L2 table, or some kind of bid/ask spread chart
- Replace shared_buffer (std:string) with a ring buffer for the reader/processor hand-off. Fixes some race conditions and avoids locks.
- Improved input validation
- Handle Market orders. Currently, everything is treated like a limit order.
- Client-side Cancel/Modify support. LOB already has cancelOrder, but client has no way to trigger it
- Support multiple symbols. Currently, only handles a single ticker. Add multi-ticker support