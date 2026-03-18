#include <Python.h>


/* Pre 3.12 version */
/**
 * print_python_int - func
 * @p: PyObject
 */
void print_python_int(PyObject *p)
{
	char sign = 0;
	Py_ssize_t i = 0, len = 0;
	unsigned long int n = 0, pow = 0, prod = 0;
	PyLongObject *lob = NULL;
	digit *dig;

	/* printf("[*] Python integer info\n"); */
	if (!PyLong_Check(p))
	{
		/* printf("  [ERROR] Invalid List Object\n"); */
		printf("Invalid Int Object\n");
		return;
	}
	lob = (PyLongObject *)p;
	len = *(Py_ssize_t *)((char *)lob + sizeof(PyObject));
	if (len < 0)
	{
		len *= -1;
		sign = '-';
	}
	dig = (digit *)((char *)lob + sizeof(PyObject) + sizeof(Py_ssize_t));;
	if (!len)
		n = 0;
	else
	{
		for (; i < len; i++)
		{
			pow = (1UL << (PyLong_SHIFT * i));
			prod = (unsigned long)dig[i] * pow;
			if ((dig[i] && (pow > ULLONG_MAX / dig[i])) ||
				(prod > (ULONG_MAX - n)))
			{
				printf("C unsigned long int overflow\n");
				return;
			}
			n += prod;
		}
	}
	if (sign)
	{
		printf("%c", sign);
		fflush(NULL);
	}
	printf("%lu\n", n);
}
