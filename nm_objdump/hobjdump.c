#include "hobjdump.h"

/**
 * process_32 - Processes a 32-bit ELF file for objdump.
 * @map: The memory map of the ELF file.
 * @filename: The name of the file.
 *
 * Return: 0 on success, 1 on failure.
 */
int process_32(void *map, const char *filename)
{
	Elf32_Ehdr *e = (Elf32_Ehdr *)map;
	int m = e->e_ident[EI_DATA] == ELFDATA2MSB;
	Elf32_Shdr *s = (Elf32_Shdr *)((char *)map + g32(e->e_shoff, m));
	char *strtab = (char *)map + g32(s[g16(e->e_shstrndx, m)].sh_offset, m);
	uint32_t flags = D_PAGED | EXEC_P | HAS_SYMS;
	char *name;
	int i;

	if (g16(e->e_type, m) == ET_DYN)
		flags = D_PAGED | DYNAMIC | HAS_SYMS;
	else if (g16(e->e_type, m) == ET_REL)
		flags = HAS_RELOC | HAS_SYMS;
	printf("\n%s:     file format %s\n", filename,
	       get_fmt(e->e_ident[EI_CLASS], m, g16(e->e_machine, m)));
	printf("architecture: %s, flags 0x%08x:\n",
	       get_arch(g16(e->e_machine, m)), flags);
	print_flags(flags);
	printf("start address 0x%08x\n\n", g32(e->e_entry, m));
	for (i = 0; i < g16(e->e_shnum, m); i++)
	{
		name = strtab + g32(s[i].sh_name, m);
		if (g32(s[i].sh_size, m) == 0 || g32(s[i].sh_type, m) == SHT_NOBITS)
			continue;
		if (strcmp(name, ".shstrtab") == 0 || strcmp(name, ".symtab") == 0 ||
		    strcmp(name, ".strtab") == 0)
			continue;
		if (name[0] == '\0')
			continue;
		printf("Contents of section %s:\n", name);
		print_hex_ascii((unsigned char *)map + g32(s[i].sh_offset, m),
				g32(s[i].sh_size, m), g32(s[i].sh_addr, m));
	}
	return (0);
}

/**
 * process_64 - Processes a 64-bit ELF file for objdump.
 * @map: The memory map of the ELF file.
 * @filename: The name of the file.
 *
 * Return: 0 on success, 1 on failure.
 */
int process_64(void *map, const char *filename)
{
	Elf64_Ehdr *e = (Elf64_Ehdr *)map;
	int m = e->e_ident[EI_DATA] == ELFDATA2MSB;
	Elf64_Shdr *s = (Elf64_Shdr *)((char *)map + g64(e->e_shoff, m));
	char *strtab = (char *)map + g64(s[g16(e->e_shstrndx, m)].sh_offset, m);
	uint32_t flags = D_PAGED | EXEC_P | HAS_SYMS;
	char *name;
	int i;

	if (g16(e->e_type, m) == ET_DYN)
		flags = D_PAGED | DYNAMIC | HAS_SYMS;
	else if (g16(e->e_type, m) == ET_REL)
		flags = HAS_RELOC | HAS_SYMS;
	printf("\n%s:     file format %s\n", filename,
	       get_fmt(e->e_ident[EI_CLASS], m, g16(e->e_machine, m)));
	printf("architecture: %s, flags 0x%08x:\n",
	       get_arch(g16(e->e_machine, m)), flags);
	print_flags(flags);
	printf("start address 0x%016lx\n\n", (unsigned long)g64(e->e_entry, m));
	for (i = 0; i < g16(e->e_shnum, m); i++)
	{
		name = strtab + g32(s[i].sh_name, m);
		if (g64(s[i].sh_size, m) == 0 || g32(s[i].sh_type, m) == SHT_NOBITS)
			continue;
		if (strcmp(name, ".shstrtab") == 0 || strcmp(name, ".symtab") == 0 ||
		    strcmp(name, ".strtab") == 0 || strcmp(name, ".bss") == 0)
			continue;
		if (name[0] == '\0')
			continue;
		printf("Contents of section %s:\n", name);
		print_hex_ascii((unsigned char *)map + g64(s[i].sh_offset, m),
				g64(s[i].sh_size, m), g64(s[i].sh_addr, m) != 0 ?
				g64(s[i].sh_addr, m) : 0);
	}
	return (0);
}

/**
 * process_file - Opens and mmaps the file for processing.
 * @filename: The name of the file.
 *
 * Return: 0 on success, 1 on failure.
 */
int process_file(const char *filename)
{
	int fd, err = 0;
	struct stat st;
	void *map;
	unsigned char *ident;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		fprintf(stderr, "hobjdump: '%s': No such file\n", filename);
		return (1);
	}
	if (fstat(fd, &st) < 0)
	{
		close(fd);
		return (1);
	}
	map = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	if (map == MAP_FAILED)
	{
		close(fd);
		return (1);
	}
	ident = (unsigned char *)map;
	if (ident[0] != 0x7f || ident[1] != 'E' || ident[2] != 'L' ||
	    ident[3] != 'F')
	{
		fprintf(stderr, "hobjdump: %s: File format not recognized\n", filename);
		err = 1;
	}
	else
	{
		if (ident[EI_CLASS] == ELFCLASS32)
			err = process_32(map, filename);
		else if (ident[EI_CLASS] == ELFCLASS64)
			err = process_64(map, filename);
	}
	munmap(map, st.st_size);
	close(fd);
	return (err);
}

/**
 * main - The main entry point of the hobjdump tool.
 * @argc: The number of arguments.
 * @argv: The arguments array.
 *
 * Return: 0 on success, 1 on failure.
 */
int main(int argc, char **argv)
{
	int err = 0, i;

	if (argc == 1)
		return (process_file("a.out"));

	for (i = 1; i < argc; i++)
	{
		if (process_file(argv[i]) != 0)
			err = 1;
	}
	return (err);
}
