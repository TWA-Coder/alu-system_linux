#include "hnm.h"

/**
 * print_syms_32 - Prints the symbols for a 32-bit ELF file.
 * @map: The mapped memory of the ELF file.
 * @msb: Endianness flag.
 */
void print_syms_32(void *map, int msb)
{
	Elf32_Ehdr *e = (Elf32_Ehdr *)map;
	Elf32_Shdr *s = (Elf32_Shdr *)((char *)map + g32(e->e_shoff, msb));
	Elf32_Sym *sym = NULL;
	char *str = NULL, *name;
	int num = 0, i;
	char type;

	for (i = 0; i < g16(e->e_shnum, msb); i++)
	{
		if (g32(s[i].sh_type, msb) == SHT_SYMTAB)
		{
			sym = (Elf32_Sym *)((char *)map + g32(s[i].sh_offset, msb));
			num = g32(s[i].sh_size, msb) / sizeof(Elf32_Sym);
			str = (char *)map + g32(s[g32(s[i].sh_link, msb)].sh_offset, msb);
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
		name = str + g32(sym[i].st_name, msb);
		if (!name || name[0] == '\0' ||
		    ELF32_ST_TYPE(sym[i].st_info) == STT_SECTION ||
		    ELF32_ST_TYPE(sym[i].st_info) == STT_FILE)
			continue;
		type = get_sym_type_32(&sym[i], s, msb);
		if (g16(sym[i].st_shndx, msb) == SHN_UNDEF || type == 'U' ||
		    type == 'w' || type == 'v')
			printf("         %c %s\n", type, name);
		else
			printf("%08x %c %s\n", g32(sym[i].st_value, msb), type, name);
	}
}

/**
 * print_syms_64 - Prints the symbols for a 64-bit ELF file.
 * @map: The mapped memory of the ELF file.
 * @msb: Endianness flag.
 */
void print_syms_64(void *map, int msb)
{
	Elf64_Ehdr *e = (Elf64_Ehdr *)map;
	Elf64_Shdr *s = (Elf64_Shdr *)((char *)map + g64(e->e_shoff, msb));
	Elf64_Sym *sym = NULL;
	char *str = NULL, *name;
	int num = 0, i;
	char type;

	for (i = 0; i < g16(e->e_shnum, msb); i++)
	{
		if (g32(s[i].sh_type, msb) == SHT_SYMTAB)
		{
			sym = (Elf64_Sym *)((char *)map + g64(s[i].sh_offset, msb));
			num = g64(s[i].sh_size, msb) / sizeof(Elf64_Sym);
			str = (char *)map + g64(s[g32(s[i].sh_link, msb)].sh_offset, msb);
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
		name = str + g32(sym[i].st_name, msb);
		if (!name || name[0] == '\0' ||
		    ELF64_ST_TYPE(sym[i].st_info) == STT_SECTION ||
		    ELF64_ST_TYPE(sym[i].st_info) == STT_FILE)
			continue;
		type = get_sym_type_64(&sym[i], s, msb);
		if (g16(sym[i].st_shndx, msb) == SHN_UNDEF || type == 'U' ||
		    type == 'w' || type == 'v')
			printf("                 %c %s\n", type, name);
		else
			printf("%016lx %c %s\n", g64(sym[i].st_value, msb), type, name);
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
	int fd, err = 0, msb;
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
		msb = ident[EI_DATA] == ELFDATA2MSB;
		if (ident[EI_CLASS] == ELFCLASS32)
			print_syms_32(map, msb);
		else if (ident[EI_CLASS] == ELFCLASS64)
			print_syms_64(map, msb);
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
