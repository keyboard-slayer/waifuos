#include <kawaii-embed/debug.hpp>
#include <kawaii-embed/loader.hpp>

#include "term.hpp"

extern "C" void
bootstrap()
{
	Framebuffer fb =
		loader_get_fb().unwrap_or_else([]() { panic("No framebuffer"); });

	LoaderModule module =
		loader_get_module("/media/aqua.tga")
			.unwrap_or_else([]() { panic("No module found"); });

	Term term(fb, module);

	for (;;)
		;
}