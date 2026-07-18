#ifndef RING_BUFFER_H
#define RING_BUFFER_H
#include <atomic>
#include <vector>


class RingBuffer {
private:
	std::atomic<int> read_ind {};
	std::atomic<int> write_ind {};
	std::vector<std::string> buffer;

public:
	RingBuffer(size_t cap);
	std::string& read();
	void write(std::string);
	bool isEmpty();
	bool isFull();
};

#endif