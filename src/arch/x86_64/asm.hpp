#pragma once

#include <stdint.h>

inline void
out8(uint16_t port, uint8_t data)
{
	asm volatile("outb %0, %1" : : "a"(data), "Nd"(port));
}

inline uint8_t
in8(uint16_t port)
{
	uint8_t data;
	asm volatile("inb %1, %0" : "=a"(data) : "Nd"(port));
	return data;
}
