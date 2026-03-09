#include "hobjdump.h"

/**
 * g16 - Get 16-bit integer, swapping if msb.
 * @x: The value.
 * @msb: Big-endian flag.
 * Return: the value.
 */
uint16_t g16(uint16_t x, int msb)
{
	if (!msb)
		return (x);
	return (((x >> 8) & 0xff) | ((x & 0xff) << 8));
}

/**
 * g32 - Get 32-bit integer, swapping if msb.
 * @x: The value.
 * @msb: Big-endian flag.
 * Return: the value.
 */
uint32_t g32(uint32_t x, int msb)
{
	if (!msb)
		return (x);
	return (((x >> 24) & 0xff) | ((x >> 8) & 0xff00) |
		((x & 0xff00) << 8) | ((x & 0xff) << 24));
}

/**
 * g64 - Get 64-bit integer, swapping if msb.
 * @x: The value.
 * @msb: Big-endian flag.
 * Return: the value.
 */
uint64_t g64(uint64_t x, int msb)
{
	if (!msb)
		return (x);
	return (((x >> 56) & 0xff) | ((x >> 40) & 0xff00) |
		((x >> 24) & 0xff0000) | ((x >> 8) & 0xff000000) |
		((x & 0xff000000) << 8) | ((x & 0xff0000) << 24) |
		((x & 0xff00) << 40) | ((x & 0xff) << 56));
}

/**
 * get_fmt - Get BFD format string.
 * @cls: ELF class.
 * @msb: Endianness.
 * @mach: Machine architecture.
 * Return: format string.
 */
const char *get_fmt(int cls, int msb, int mach)
{
	if (cls == ELFCLASS32)
	{
		if (mach == EM_386)
			return ("elf32-i386");
		if (mach == EM_SPARC || mach == EM_SPARC32PLUS)
			return ("elf32-sparc");
		return (msb ? "elf32-big" : "elf32-little");
	}
	else
	{
		if (mach == EM_X86_64)
			return ("elf64-x86-64");
		if (mach == EM_SPARCV9)
			return ("elf64-sparc");
		return (msb ? "elf64-big" : "elf64-little");
	}
}

/**
 * get_arch - Get BFD arch string.
 * @mach: Machine architecture.
 * Return: architecture string.
 */
const char *get_arch(int mach)
{
	if (mach == EM_386)
		return ("i386");
	if (mach == EM_X86_64)
		return ("i386:x86-64");
	if (mach == EM_SPARC || mach == EM_SPARC32PLUS || mach == EM_SPARCV9)
		return ("sparc");
	return ("UNKNOWN!");
}
