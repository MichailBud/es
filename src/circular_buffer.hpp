#pragma once // Компиляторы майкрософт проверяют есть ли уже включенный такой файл, чтобы заново его не включать
#include <stdint.h>
#include <cstdint>

constexpr uint8_t SIZE{32};

class Circular_buffer {
public:
	void put(uint8_t);
	uint8_t get();
	bool empty();
	bool full();

	Circular_buffer();        // Circular_buffer b1;

private:
	uint8_t buf[32];
	uint8_t wr_idx;
	uint8_t rd_idx;
	bool full_;
};