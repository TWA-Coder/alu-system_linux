CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic

all: hnm hobjdump

hnm: hnm.c hnm_type.c
	$(CC) $(CFLAGS) hnm.c hnm_type.c -o hnm

hobjdump: hobjdump.c hobjdump_print.c
	$(CC) $(CFLAGS) hobjdump.c hobjdump_print.c -o hobjdump

clean:
	rm -f hnm hobjdump
