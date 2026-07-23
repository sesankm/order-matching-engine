# order matching engine
A TCP-based order matching engine using a Limit Order Book (LOB) to process buy and sell orders based on price-time priority.

## Overview
- **Server** - accepts multiple TCP connections, each connection handled in it's own detached reader thread, single processor thread that feeds orders into the matching engine after reading from a shared buffer
- **Client** - reads orders from an input file and sends them over a single TCP socket, one thread per message
- **Limit Order Book (LOB)** - stores buy and sell orders. No execution logic
- **Matching engine** - sits above the LOB, detects crossed orders and fills. This is the entry point for order submissions.

<img width="645" height="576" alt="image" src="https://github.com/user-attachments/assets/8df35d79-aa2f-4336-a2d9-d8bc76b13576" />

(each reader writes to one slot in buffer per message and can receive multiple messages)


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
- Handle Market orders. Currently, everything is treated like a limit order.
- Client-side Cancel/Modify support. LOB already has cancelOrder, but client has no way to trigger it
- Support multiple symbols. Currently, only handles a single ticker. Add multi-ticker support
