#pragma once

#include <stddef.h>
#include <stdint.h>

struct Framebuffer
{
	uintptr_t addr;
	uint64_t width;
	uint64_t height;
	uint64_t pitch;
	uint64_t bpp;

	Framebuffer(uintptr_t _addr, uint64_t _width, uint64_t _height,
				uint64_t _pitch, uint64_t _bpp)
		: addr(_addr), width(_width), height(_height), pitch(_pitch), bpp(_bpp)
	{
	}

	Framebuffer() = default;

	void
	put_pixel(uint64_t x, uint64_t y, uint32_t color)
	{
		size_t index = x + (pitch / sizeof(uint32_t)) * y;
		((uint32_t *) addr)[index] = color;
	}

	void
	put_rect(size_t x, size_t y, size_t w, size_t h, uint32_t color)
	{
		for (size_t i = 0; i < w; i++)
		{
			for (size_t j = 0; j < h; j++)
			{
				put_pixel(x + i, y + j, color);
			}
		}
	}
};