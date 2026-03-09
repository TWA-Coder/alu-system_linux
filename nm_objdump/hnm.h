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
char get_sym_type_32(Elf32_Sym *sym, Elf32_Shdr *shdr);
char get_sym_type_64(Elf64_Sym *sym, Elf64_Shdr *shdr);
void print_syms_32(void *map);
void print_syms_64(void *map);
int process_file(const char *filename, int multi);

#endif /* HNM_H */
