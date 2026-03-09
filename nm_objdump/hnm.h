#ifndef HNM_H
#define HNM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <elf.h>

char get_sym_type_common(int bind, int type, uint16_t shndx, uint32_t sh_type,
			 uint64_t sh_flags);
char get_sym_type_32(Elf32_Sym *sym, Elf32_Shdr *shdr, int msb);
char get_sym_type_64(Elf64_Sym *sym, Elf64_Shdr *shdr, int msb);
void print_syms_32(void *map, int msb);
void print_syms_64(void *map, int msb);
int process_file(const char *filename, int multi);
uint16_t g16(uint16_t x, int msb);
uint32_t g32(uint32_t x, int msb);
uint64_t g64(uint64_t x, int msb);

#endif /* HNM_H */
