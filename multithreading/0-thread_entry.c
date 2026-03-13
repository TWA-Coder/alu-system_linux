#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "multithreading.h"

/**
 * thread_entry - serves as the entry point to a new thread
 * @arg: holds the address of a string that must be printed
 *
 * Return: NULL
 */
void *thread_entry(void *arg)
{
	printf("%s\n", (char *)arg);
	pthread_exit(NULL);
	return (NULL);
}
