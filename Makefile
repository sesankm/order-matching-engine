CXX	  = clang++
CXX_FLAGS = -std=c++20 -g


matchbook: main.cpp client.cpp
	$(CXX) $(CXX_FLAGS) main.cpp client.cpp -o matchbook

clean:
	rm -rf matchbook matchbook.dSYM
