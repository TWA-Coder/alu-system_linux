#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>
#include "multithreading.h"

static pthread_mutex_t print_mutex;

/**
 * tprintf_init - initializes the mutex before main is executed
 */
void __attribute__((constructor)) tprintf_init(void)
{
	pthread_mutex_init(&print_mutex, NULL);
}

/**
 * tprintf_destroy - destroys the mutex after main returns
 */
void __attribute__((destructor)) tprintf_destroy(void)
{
	pthread_mutex_destroy(&print_mutex);
}

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

	pthread_mutex_lock(&print_mutex);

	printf("[%lu] ", (unsigned long)pthread_self());

	va_start(args, format);
	printed_chars = vprintf(format, args);
	va_end(args);

	pthread_mutex_unlock(&print_mutex);

	return (printed_chars);
}
