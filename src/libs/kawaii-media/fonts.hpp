#pragma once

#include <kawaii-base/slice.hpp>

struct Font
{
	size_t width;
	size_t height;
};

struct BitmapFont : public Font
{
	Bytes data;
};