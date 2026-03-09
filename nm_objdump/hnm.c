#include "hnm.h"

/**
 * print_syms_32 - Prints the symbols for a 32-bit ELF file.
 * @map: The mapped memory of the ELF file.
 */
void print_syms_32(void *map)
{
	Elf32_Ehdr *e = (Elf32_Ehdr *)map;
	Elf32_Shdr *s = (Elf32_Shdr *)((char *)map + e->e_shoff);
	Elf32_Sym *sym = NULL;
	char *str = NULL, *name;
	int num = 0, i;
	char type;

	for (i = 0; i < e->e_shnum; i++)
	{
		if (s[i].sh_type == SHT_SYMTAB)
		{
			sym = (Elf32_Sym *)((char *)map + s[i].sh_offset);
			num = s[i].sh_size / sizeof(Elf32_Sym);
			str = (char *)map + s[s[i].sh_link].sh_offset;
			break;
		}
	}
	if (!sym || !str)
	{
		fprintf(stderr, "hnm: no symbols\n");
		return;
	}
	for (i = 0; i < num; i++)
	{
		name = str + sym[i].st_name;
		if (!name || name[0] == '\0' ||
		    ELF32_ST_TYPE(sym[i].st_info) == STT_SECTION ||
		    ELF32_ST_TYPE(sym[i].st_info) == STT_FILE)
			continue;
		type = get_sym_type_32(&sym[i], s);
		if (sym[i].st_shndx == SHN_UNDEF || type == 'U' || type == 'w' ||
		    type == 'v')
			printf("         %c %s\n", type, name);
		else
			printf("%08x %c %s\n", sym[i].st_value, type, name);
	}
}

/**
 * print_syms_64 - Prints the symbols for a 64-bit ELF file.
 * @map: The mapped memory of the ELF file.
 */
void print_syms_64(void *map)
{
	Elf64_Ehdr *e = (Elf64_Ehdr *)map;
	Elf64_Shdr *s = (Elf64_Shdr *)((char *)map + e->e_shoff);
	Elf64_Sym *sym = NULL;
	char *str = NULL, *name;
	int num = 0, i;
	char type;

	for (i = 0; i < e->e_shnum; i++)
	{
		if (s[i].sh_type == SHT_SYMTAB)
		{
			sym = (Elf64_Sym *)((char *)map + s[i].sh_offset);
			num = s[i].sh_size / sizeof(Elf64_Sym);
			str = (char *)map + s[s[i].sh_link].sh_offset;
			break;
		}
	}
	if (!sym || !str)
	{
		fprintf(stderr, "hnm: no symbols\n");
		return;
	}
	for (i = 0; i < num; i++)
	{
		name = str + sym[i].st_name;
		if (!name || name[0] == '\0' ||
		    ELF64_ST_TYPE(sym[i].st_info) == STT_SECTION ||
		    ELF64_ST_TYPE(sym[i].st_info) == STT_FILE)
			continue;
		type = get_sym_type_64(&sym[i], s);
		if (sym[i].st_shndx == SHN_UNDEF || type == 'U' || type == 'w' ||
		    type == 'v')
			printf("                 %c %s\n", type, name);
		else
			printf("%016lx %c %s\n", sym[i].st_value, type, name);
	}
}

/**
 * process_file - Processes an individual ELF file.
 * @filename: The name of the file to process.
 * @multi: Flag indicating if multiple files are processed.
 *
 * Return: 0 on success, 1 on failure.
 */
int process_file(const char *filename, int multi)
{
	int fd, err = 0;
	struct stat st;
	void *map;
	unsigned char *ident;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		fprintf(stderr, "hnm: '%s': No such file\n", filename);
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
		fprintf(stderr, "hnm: %s: file format not recognized\n", filename);
		err = 1;
	}
	else
	{
		if (multi)
			printf("\n%s:\n", filename);
		if (ident[EI_CLASS] == ELFCLASS32)
			print_syms_32(map);
		else if (ident[EI_CLASS] == ELFCLASS64)
			print_syms_64(map);
	}
	munmap(map, st.st_size);
	close(fd);
	return (err);
}

/**
 * main - The entry point for the hnm program.
 * @argc: The number of arguments.
 * @argv: The arguments array.
 *
 * Return: 0 on success, 1 on error.
 */
int main(int argc, char **argv)
{
	int err = 0, multi, i;

	if (argc == 1)
		return (process_file("a.out", 0));

	multi = (argc > 2);
	for (i = 1; i < argc; i++)
	{
		if (process_file(argv[i], multi) != 0)
			err = 1;
	}
	return (err);
}
