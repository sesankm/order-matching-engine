#include "ring_buffer.hpp"
#include <optional>

RingBuffer::RingBuffer(size_t cap) 
	: read_ind {0}
	, write_ind{0}
	, buffer(cap + 1, std::string{}) {}

std::optional<std::string> RingBuffer::read() {
	if (!isEmpty()) {
		std::string& ret = buffer.at(read_ind);
		read_ind = (read_ind + 1) % buffer.size();
		return ret;
	} 
	return std::nullopt;
}

void RingBuffer::write(const std::string& input) {
	if (!isFull()) {
		buffer[write_ind] = input;
		write_ind = (write_ind + 1) % buffer.size();
	}
}

bool RingBuffer::isEmpty() const {
	return read_ind == write_ind;
}

bool RingBuffer::isFull() const {
	return (write_ind + 1) % buffer.size() == read_ind;
}

