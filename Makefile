CXX	  = clang++
CXX_FLAGS = -std=c++20


matchbook: main.cpp
	$(CXX) $(CXX_FLAGS) main.cpp -o matchbook

clean:
	rm -rf matchbook matchbook.dSYM
