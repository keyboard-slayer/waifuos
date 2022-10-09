#pragma once

#include <stddef.h>
#include <stdint.h>

#include "maybe.hpp"

template <typename T> struct Slice
{
	T *data;
	Maybe<size_t> size;

	Slice(T *data, size_t size) : data(data), size(size) {}
	Slice(T *data) : data(data), size() {}

	T *
	get_data() const
	{
		return data;
	}

	Maybe<size_t>
	get_size() const
	{
		return size;
	}

	T &
	operator[](size_t index) const
	{
		return data[index];
	}

	bool
	operator==(Slice<T> other) const
	{
		if (!size.has_value || !other.size.has_value)
		{
			return false;
		}

		for (size_t i = 0; i < size.value; i++)
		{
			if (data[i] != other.data[i])
			{
				return false;
			}
		}

		return true;
	}

	T *
	begin() const
	{
		return data;
	}

	T *
	end() const
	{
		return data + size.value;
	}
};

struct String : public Slice<char>
{
	String(char *data, size_t size) : Slice<char>(data, size) {}
	String(char *data) : Slice<char>(data)
	{
		size_t i;
		for (i = 0; data[i] != '\0'; i++)
			;

		size = i;
	}
};

using Byte = uint8_t;
using Bytes = Slice<Byte>;