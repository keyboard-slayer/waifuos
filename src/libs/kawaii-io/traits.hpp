#pragma once

#include <kawaii-base/maybe.hpp>
#include <kawaii-base/slice.hpp>

struct Writer
{
	virtual void putc(Byte c) = 0;
	virtual Maybe<size_t> puts(Bytes str) = 0;
};