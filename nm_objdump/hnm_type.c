#include "hnm.h"

/**
 * get_sym_type_common - Gets the character character for the symbol type.
 * @bind: The symbol binding.
 * @type: The symbol type.
 * @shndx: The section header index of the symbol.
 * @sh_type: The section type.
 * @sh_flags: The section flags.
 *
 * Return: The character representing the symbol type.
 */
char get_sym_type_common(int bind, int type, uint16_t shndx, uint32_t sh_type,
			 uint64_t sh_flags)
{
	char c = '?';

	if (bind == STB_GNU_UNIQUE)
		c = 'u';
	else if (bind == STB_WEAK)
	{
		c = 'W';
		if (shndx == SHN_UNDEF)
			c = 'w';
		if (type == STT_OBJECT)
		{
			c = 'V';
			if (shndx == SHN_UNDEF)
				c = 'v';
		}
		return (c);
	}
	else if (shndx == SHN_UNDEF)
		c = 'U';
	else if (shndx == SHN_ABS)
		c = 'A';
	else if (shndx == SHN_COMMON)
		c = 'C';
	else if (sh_type == SHT_NOBITS)
		c = 'B';
	else if (sh_flags & SHF_EXECINSTR)
		c = 'T';
	else if ((sh_flags & SHF_ALLOC) && (sh_flags & SHF_WRITE))
		c = 'D';
	else if (sh_flags & SHF_ALLOC)
		c = 'R';
	else if (type == STT_FILE)
		c = 'f';
	else if (sh_type == SHT_PROGBITS || sh_type == SHT_DYNAMIC)
		c = 't';
	else
		c = 'n';
	if (bind == STB_LOCAL && c >= 'A' && c <= 'Z')
		c += 32;
	return (c);
}

/**
 * get_sym_type_32 - Gets the symbol type character for 32-bit ELF.
 * @sym: The 32-bit symbol.
 * @shdr: The 32-bit section headers.
 *
 * Return: The character representing the symbol type.
 */
char get_sym_type_32(Elf32_Sym *sym, Elf32_Shdr *shdr)
{
	int bind = ELF32_ST_BIND(sym->st_info);
	int type = ELF32_ST_TYPE(sym->st_info);
	uint32_t sh_t = 0, sh_f = 0;

	if (sym->st_shndx < SHN_LORESERVE)
	{
		sh_t = shdr[sym->st_shndx].sh_type;
		sh_f = shdr[sym->st_shndx].sh_flags;
	}

	return (get_sym_type_common(bind, type, sym->st_shndx, sh_t, sh_f));
}

/**
 * get_sym_type_64 - Gets the symbol type character for 64-bit ELF.
 * @sym: The 64-bit symbol.
 * @shdr: The 64-bit section headers.
 *
 * Return: The character representing the symbol type.
 */
char get_sym_type_64(Elf64_Sym *sym, Elf64_Shdr *shdr)
{
	int bind = ELF64_ST_BIND(sym->st_info);
	int type = ELF64_ST_TYPE(sym->st_info);
	uint32_t sh_t = 0;
	uint64_t sh_f = 0;

	if (sym->st_shndx < SHN_LORESERVE)
	{
		sh_t = shdr[sym->st_shndx].sh_type;
		sh_f = shdr[sym->st_shndx].sh_flags;
	}

	return (get_sym_type_common(bind, type, sym->st_shndx, sh_t, sh_f));
}
