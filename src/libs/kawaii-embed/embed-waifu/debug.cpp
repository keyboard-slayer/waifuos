#include <arch/arch.hpp>
#include <kawaii-embed/debug.hpp>

#include ARCH(com.hpp)

static Com com(__CONFIG_SERIAL__, __CONFIG_BAUDS__);

[[noreturn]] void
panic(const char *fmt, ...)
{
	com.puts(fmt);

	asm volatile("cli");
	asm volatile("hlt");
	__builtin_unreachable();
}