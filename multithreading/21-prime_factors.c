#include <stdlib.h>
#include "multithreading.h"

/**
 * prime_factors - factorizes a number into a list of prime factors
 * @s: string representation of the number to factorize
 *
 * Return: pointer to the list of prime factors
 */
list_t *prime_factors(char const *s)
{
	unsigned long n = strtoul(s, NULL, 10);
	unsigned long i;
	unsigned long *factor;
	list_t *factors = calloc(1, sizeof(list_t));

	if (!factors)
		return (NULL);
	list_init(factors);

	while (n % 2 == 0)
	{
		factor = malloc(sizeof(*factor));
		if (factor)
		{
			*factor = 2;
			list_add(factors, factor);
		}
		n /= 2;
	}

	for (i = 3; i * i <= n; i += 2)
	{
		while (n % i == 0)
		{
			factor = malloc(sizeof(*factor));
			if (factor)
			{
				*factor = i;
				list_add(factors, factor);
			}
			n /= i;
		}
	}

	if (n > 2)
	{
		factor = malloc(sizeof(*factor));
		if (factor)
		{
			*factor = n;
			list_add(factors, factor);
		}
	}

	return (factors);
}
