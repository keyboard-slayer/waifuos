#pragma once

#include <kawaii-base/func.hpp>

template <typename T> struct Maybe
{
	Maybe() : has_value(false), value() {}
	Maybe(T value) : has_value(true), value(value) {}

	template <typename Func>
	auto
	operator>>=(Func fn) const
	{
		if (has_value)
		{
			return Maybe<decltype(fn(value))>(fn(value));
		}
		else
		{
			return Maybe<decltype(fn(value))>();
		}
	}

	auto
	unwrap_or_else(Func<void()> fn) const
	{
		if (has_value)
		{
			return value;
		}
		else
		{
			fn();
			return value;
		}
	}

	bool has_value;
	T value;
};