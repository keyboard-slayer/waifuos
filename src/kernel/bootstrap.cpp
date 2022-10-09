#include <arch/arch.hpp>
#include ARCH(com.hpp)

#include <kawaii-abstract/loader.hpp>

#include "term.hpp"

extern "C" void
bootstrap()
{
	Com com(COM1);

	Framebuffer fb = loader_get_fb().unwrap_or_else(
		[&com]
		{
			com.puts("No framebuffer found!\n");
			for (;;)
				;
		});

	LoaderModule module = loader_get_module("/media/aqua.tga")
							  .unwrap_or_else(
								  [&com]
								  {
									  com.puts("Couldn't find aqua.tga!\n");
									  for (;;)
										  ;
								  });

	Term term(fb, module);

	for (;;)
		;
}