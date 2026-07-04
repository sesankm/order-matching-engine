CXX	  = clang++
CXX_FLAGS = -std=c++20 -g -Iinclude
SRC	  = $(wildcard src/*.cpp)


matchbook: $(SRC)
	$(CXX) $(CXX_FLAGS) $(SRC) -o matchbook

clean:
	rm -rf matchbook matchbook.dSYM
