#include "ring_buffer.hpp"


RingBuffer::RingBuffer(size_t cap) 
	: read_ind {0}
	, write_ind{0}
	, buffer(std::vector<std::string>(cap, std::string{})) {}

std::string& RingBuffer::read() {
	std::string& ret = buffer.at(read_ind);
	read_ind = (read_ind + 1) % buffer.size();
	return ret;
}

void RingBuffer::write(std::string input) {
	buffer[write_ind] = input;
	write_ind = (write_ind + 1) % buffer.size();
}