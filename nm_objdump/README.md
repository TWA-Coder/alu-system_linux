# ELF, nm, and objdump

This repository and its subdirectories contain information and exercises relating to the ELF format and the `nm` and `objdump` tools.

## What is the ELF format?
ELF (Executable and Linkable Format) is a common standard file format for executable files, object code, shared libraries, and core dumps in Unix and Unix-like systems. It provides a structured way to store the code, data, and metadata (such as symbols and relocation information) necessary for a program to be loaded and executed by the operating system.

## What are the commands nm and objdump?
- **`nm`**: A powerful command-line utility used to list the symbols from object files. It displays the symbol table, providing information about functions and variables within an executable or object file.
- **`objdump`**: A comprehensive program used to display information about object files. It is often used as a disassembler to view the machine-level instructions in an executable, but it can also display section headers, symbol tables, relocation entries, and more.

## How to use them properly
### `nm` usage
To use `nm`, simply pass the object file or executable as an argument:
```bash
nm ./executable_file
```
Common options:
- `-A`, `-o`: Print the name of the input file before each symbol.
- `-a`: Display all symbols, including debugger-only symbols.
- `-g`: Display only external (global) symbols.
- `-u`: Display only undefined symbols.

### `objdump` usage
To display the contents of an object file using `objdump`:
```bash
objdump [options] ./executable_file
```
Common options:
- `-d`: Disassemble the executable sections (code).
- `-D`: Disassemble all sections.
- `-x`: Display all available header information.
- `-s`: Display the full contents of all sections.
- `-S`: Intermix source code with disassembly (requires code compiled with debug symbols, e.g., `-g`).

## How to parse the content of an ELF file
Parsing an ELF file involves reading its structured data components. The general structure of an ELF file includes:
1. **ELF Header**: Contains basic information about the file like its format (32/64 bit), endianness, machine architecture, and the entry point address.
2. **Program Header Table**: Explains how the system should create a process image. It's essentially a list of segments to be loaded into memory.
3. **Section Header Table**: Defines the different sections of the file (e.g., `.text` for code, `.data` for initialized data, `.bss` for uninitialized data).
4. **Data**: The actual code, data, symbol tables, and other structures referenced by the headers.

To parse it manually or programmatically, you would read the ELF header to find the location and size of the program and section header tables, and then use those tables to locate and read specific sections or segments.

## What information can be extracted from an ELF file?
You can extract a wide range of information from an ELF file, including:
- **Executable Instructions**: The disassembled machine code (e.g., in the `.text` section).
- **Symbol Tables**: Lists of global and local variables, and functions, along with their addresses (e.g., `.symtab` and `.dynsym`).
- **Relocation Entries**: Information needed to link an object file into an executable.
- **Data Sections**: Initialized read-only `.rodata` and read/write `.data` values, as well as uninitialized `.bss` data definitions.
- **Dynamic Linking Information**: Details about required shared libraries and dynamic symbols.
- **Debugging Information**: If compiled with debug flags, the file includes DWARF information detailing source lines, variable types, and original source code mapping.

## What are the differences between nm and objdump?
While both tools interrogate object files and executables, they have different primary focuses:
- **Scope**: `nm` is highly specialized for extracting and printing **symbol tables** (variables and function names and their memory addresses). `objdump` is a general-purpose tool that can extract a multitude of details including headers, sections, segment structures, and debugging info.
- **Disassembly**: `objdump` is the primary tool used for disassembling binary files into human-readable assembly instructions. `nm` cannot perform disassembly.
- **Output format**: `nm` outputs a clean, simple list of symbols and their types, which is easy to pipeline and filter. `objdump` provides verbose and structured outputs of different file headers or disassembled code blocks.
