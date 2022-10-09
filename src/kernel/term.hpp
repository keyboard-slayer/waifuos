#pragma once

#include <kawaii-abstract/loader.hpp>
#include <kawaii-io/traits.hpp>
#include <kawaii-media/fonts.hpp>
#include <kawaii-media/tga.hpp>

#include "framebuffer.hpp"

#define TERM_OFFSET_X (40)
#define TERM_OFFSET_Y (40)

struct Term
{
	Framebuffer &fb;
	LoaderModule waifu;
	size_t curx;
	size_t cury;

	Term(Framebuffer &fb, LoaderModule waifu) : fb(fb), waifu(waifu)
	{
		// Draw the background
		fb.put_rect(0, 0, fb.width, fb.height, 0x666699);

		// Window shadow
		fb.put_rect(TERM_OFFSET_X - 2, TERM_OFFSET_Y - 1, fb.width - 75,
					fb.height - 76, 0);

		// Window body
		fb.put_rect(TERM_OFFSET_X, TERM_OFFSET_Y, fb.width - 80, fb.height - 80,
					0xffffff);

		// Window titlebar
		fb.put_rect(TERM_OFFSET_X + 1, TERM_OFFSET_Y + 1, fb.width - 81, 35,
					0xbbbbbb);

		fb.put_rect(TERM_OFFSET_X, TERM_OFFSET_Y + 35, fb.width - 78, 1, 0);

		// Draw the waifu
		TGAHeader *header = (TGAHeader *) waifu.ptr;

		uint32_t *img =
			(uint32_t *) ((uintptr_t) waifu.ptr + sizeof(TGAHeader));

		size_t i = header->width * header->height;

		for (short y = 0; y < header->height; y++)
		{
			for (short x = header->width; x > 0; x--)
			{
				uint32_t pixel = img[i--];

				if (pixel >> 24)
				{
					fb.put_pixel((fb.width - 300) + x, (fb.height - 420) + y,
								 pixel);
				}
			}
		}
	}
};