#pragma once

template <typename> struct Func;
template <typename R, typename... Args> struct Func<R(Args...)>
{
	void *obj = nullptr;

	R (*wrap)(void *, Args...) = nullptr;
	Func() = default;
	Func(R target(Args...))
		: Func([=](Args... args) { return target(args...); })
	{
	}

	template <typename T>
	Func(T &&fn)
		: obj(&fn),
		  wrap([](void *fn, Args... args) { return (*(T *) fn)(args...); })
	{
	}

	R
	operator()(Args... args) const
	{
		return wrap(obj, args...);
	}
};