#ifndef HOBJDUMP_H
#define HOBJDUMP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <elf.h>
#include <ctype.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

/* objdump bit flags */
#define BFD_NO_FLAGS    0x00
#define HAS_RELOC       0x01
#define EXEC_P          0x02
#define HAS_LINENO      0x04
#define HAS_DEBUG       0x08
#define HAS_SYMS        0x10
#define HAS_LOCALS      0x20
#define DYNAMIC         0x40
#define WP_TEXT         0x80
#define D_PAGED         0x100

void print_hex_ascii(unsigned char *data, size_t size, uint64_t addr);
void print_flags(uint32_t flags);
int process_32(void *map, const char *filename);
int process_64(void *map, const char *filename);
int process_file(const char *filename);
uint16_t g16(uint16_t x, int msb);
uint32_t g32(uint32_t x, int msb);
uint64_t g64(uint64_t x, int msb);
const char *get_fmt(int cls, int msb, int mach);
const char *get_arch(int mach);

#endif /* HOBJDUMP_H */
