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
	std::optional<std::string> read();
	void write(const std::string&);
	bool isEmpty() const;
	bool isFull() const;
};

#endif