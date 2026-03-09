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
	Elf32_Shdr *s = (Elf32_Shdr *)((char *)map + e->e_shoff);
	char *strtab = (char *)map + s[e->e_shstrndx].sh_offset;
	char *name;
	int i;

	printf("\n%s:     file format elf32-i386\n", filename);
	printf("architecture: i386, flags 0x%08x:\n", D_PAGED | EXEC_P | HAS_SYMS);
	print_flags(D_PAGED | EXEC_P | HAS_SYMS);
	printf("start address 0x%08x\n\n", e->e_entry);

	for (i = 0; i < e->e_shnum; i++)
	{
		name = strtab + s[i].sh_name;
		if (s[i].sh_size == 0 || s[i].sh_type == SHT_NOBITS)
			continue;
		if (strcmp(name, ".shstrtab") == 0 || strcmp(name, ".symtab") == 0 ||
		    strcmp(name, ".strtab") == 0)
			continue;
		if (name[0] == '\0')
			continue;
		printf("Contents of section %s:\n", name);
		print_hex_ascii((unsigned char *)map + s[i].sh_offset,
				s[i].sh_size, s[i].sh_addr);
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
	Elf64_Shdr *s = (Elf64_Shdr *)((char *)map + e->e_shoff);
	char *strtab = (char *)map + s[e->e_shstrndx].sh_offset;
	uint32_t flags = D_PAGED | EXEC_P | HAS_SYMS;
	char *name;
	int i;

	if (e->e_type == ET_DYN)
		flags = D_PAGED | DYNAMIC | HAS_SYMS;
	else if (e->e_type == ET_REL)
		flags = HAS_RELOC | HAS_SYMS;
	printf("\n%s:     file format elf64-x86-64\n", filename);
	printf("architecture: i386:x86-64, flags 0x%08x:\n", flags);
	print_flags(flags);
	printf("start address 0x%016lx\n\n", (unsigned long)e->e_entry);

	for (i = 0; i < e->e_shnum; i++)
	{
		name = strtab + s[i].sh_name;
		if (s[i].sh_size == 0 || s[i].sh_type == SHT_NOBITS)
			continue;
		if (strcmp(name, ".shstrtab") == 0 || strcmp(name, ".symtab") == 0 ||
		    strcmp(name, ".strtab") == 0 || strcmp(name, ".bss") == 0)
			continue;
		if (name[0] == '\0')
			continue;
		printf("Contents of section %s:\n", name);
		print_hex_ascii((unsigned char *)map + s[i].sh_offset,
				s[i].sh_size, s[i].sh_addr != 0 ? s[i].sh_addr : 0);
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
