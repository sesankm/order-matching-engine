CXX	   = clang++
CXX_FLAGS  = -std=c++20 -g -Iinclude
SERVER_SRC = src/peer.cpp src/server.cpp
CLIENT_SRC = src/peer.cpp src/client.cpp

all: server client

server: $(SERVER_SRC) src/run_server.cpp
	$(CXX) $(CXX_FLAGS)  $(SERVER_SRC) src/run_server.cpp -o server

client: $(CLIENT_SRC) src/run_client.cpp
	$(CXX) $(CXX_FLAGS) $(CLIENT_SRC) src/run_client.cpp -o client

clean:
	rm -rf server client *.dSYM
