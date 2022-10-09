#ifdef __x86_64__
# define ARCH(FILE) <arch/x86_64/FILE>
#else
# error "Unknown architecture"
#endif /* !__x86_64__ */
