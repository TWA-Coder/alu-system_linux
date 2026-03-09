#include "hnm.h"

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
