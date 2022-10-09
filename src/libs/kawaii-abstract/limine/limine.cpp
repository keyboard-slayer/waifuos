#include "limine.h"
#include "../loader.hpp"

extern "C" void bootstrap();

// clang-format off

volatile struct limine_entry_point_request entry_point = {
	LIMINE_ENTRY_POINT_REQUEST, 0, 0, bootstrap
};

volatile struct limine_framebuffer_request fb_req = {
	LIMINE_FRAMEBUFFER_REQUEST, 0, 0
};

volatile struct limine_module_request module_req = { 
    LIMINE_MODULE_REQUEST, 0,0 
};

// clang-format on

Maybe<Framebuffer>
loader_get_fb()
{
	struct limine_framebuffer *fb;

	if (fb_req.response == nullptr)
	{
		return {};
	}

	fb = fb_req.response->framebuffers[0];

	return Framebuffer((uintptr_t) fb->address, fb->width, fb->height,
					   fb->pitch, fb->bpp);
}

Maybe<LoaderModule>
loader_get_module(String path)
{
	size_t i;

	if (module_req.response == NULL)
	{
		return {};
	}

	for (i = 0; i < module_req.response->module_count; i++)
	{
		if (path == String(module_req.response->modules[i]->path))
		{
			return LoaderModule(module_req.response->modules[i]->address,
								module_req.response->modules[i]->size);
		}
	}

	return {};
}