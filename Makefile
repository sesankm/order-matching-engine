CXX	  = clang++
CXX_FLAGS = -std=c++20 -g -Wall -Werror -Wunused


matchbook:
	$(CXX) $(CXX_FLAGS) main.cpp -o matchbook

clean:
	rm -rf matchbook matchbook.dSYM
