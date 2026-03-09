#include "hobjdump.h"

/**
 * print_hex_ascii - Prints the hex and ASCII representation of data.
 * @data: The raw data to print.
 * @size: The size of the data.
 * @addr: The base address to display.
 */
void print_hex_ascii(unsigned char *data, size_t size, uint64_t addr)
{
	size_t i, j;
	unsigned char c;

	for (i = 0; i < size; i += 16)
	{
		printf(" %04lx", (unsigned long)(addr + i));
		for (j = 0; j < 16; j++)
		{
			if (j % 4 == 0)
				printf(" ");
			if (i + j < size)
				printf("%02x", data[i + j]);
			else
				printf("  ");
		}
		printf("  ");
		for (j = 0; j < 16; j++)
		{
			if (i + j < size)
			{
				c = data[i + j];
				if (isprint(c))
					printf("%c", c);
				else
					printf(".");
			}
			else
				printf(" ");
		}
		printf("\n");
	}
}

/**
 * print_flags - Prints the objdump flags symbolically.
 * @flags: The flags bitmask.
 */
void print_flags(uint32_t flags)
{
	int f = 0;

	if (flags == 0)
	{
		printf("BFD_NO_FLAGS\n");
		return;
	}
	if (flags & HAS_RELOC)
		printf("%sHAS_RELOC", f++ ? ", " : "");
	if (flags & EXEC_P)
		printf("%sEXEC_P", f++ ? ", " : "");
	if (flags & HAS_LINENO)
		printf("%sHAS_LINENO", f++ ? ", " : "");
	if (flags & HAS_DEBUG)
		printf("%sHAS_DEBUG", f++ ? ", " : "");
	if (flags & HAS_SYMS)
		printf("%sHAS_SYMS", f++ ? ", " : "");
	if (flags & HAS_LOCALS)
		printf("%sHAS_LOCALS", f++ ? ", " : "");
	if (flags & DYNAMIC)
		printf("%sDYNAMIC", f++ ? ", " : "");
	if (flags & WP_TEXT)
		printf("%sWP_TEXT", f++ ? ", " : "");
	if (flags & D_PAGED)
		printf("%sD_PAGED", f++ ? ", " : "");
	printf("\n");
}
