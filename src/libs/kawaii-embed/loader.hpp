#pragma once

#include <kawaii-base/maybe.hpp>
#include <kawaii-base/slice.hpp>
#include <kernel/framebuffer.hpp>
#include <stddef.h>

struct LoaderModule
{
	void *ptr;
	size_t size;

	LoaderModule(void *_ptr, size_t _size) : ptr(_ptr), size(_size) {}
	LoaderModule() : ptr(nullptr), size(0) {}
};

Maybe<Framebuffer> loader_get_fb();
Maybe<LoaderModule> loader_get_module(String path);