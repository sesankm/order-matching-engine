# order matching engine

An implementation of an order matching engine - the system exchanges use to match and fill buy/sell orders. This implementation uses client/server architecture with TCP sockets for order submission, a limit order book (LOB) and a matching engine with a time-price priority.

## Overview
- **Limit Order Book (LOB)** - stores buy and sell orders. No execution logic
- **Matching engine** - sits above the LOB, detects crossed orders and fills. This is the entry point for order submissions.
