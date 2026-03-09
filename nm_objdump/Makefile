CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic

all: hnm hobjdump

hnm: hnm.c hnm_type.c hnm_endian.c
	$(CC) $(CFLAGS) hnm.c hnm_type.c hnm_endian.c -o hnm

hobjdump: hobjdump.c hobjdump_print.c hobjdump_endian.c
	$(CC) $(CFLAGS) hobjdump.c hobjdump_print.c hobjdump_endian.c -o hobjdump

clean:
	rm -f hnm hobjdump
