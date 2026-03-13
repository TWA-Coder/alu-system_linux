#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>
#include "multithreading.h"

/**
 * tprintf - uses the printf family to print out a given formatted string
 * @format: formatted string to print
 *
 * Return: number of characters printed
 */
int tprintf(char const *format, ...)
{
	va_list args;
	int printed_chars;

	printf("[%lu] ", (unsigned long)pthread_self());

	va_start(args, format);
	printed_chars = vprintf(format, args);
	va_end(args);

	return (printed_chars);
}
