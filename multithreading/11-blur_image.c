#include <pthread.h>
#include <stdlib.h>
#include "multithreading.h"

#define NUM_THREADS 7

/**
 * thread_blur - entry point for blurring thread
 * @arg: pointer to the blur_portion_t structure for this thread
 *
 * Return: NULL
 */
static void *thread_blur(void *arg)
{
	blur_portion_t *portion = (blur_portion_t *)arg;

	blur_portion(portion);
	pthread_exit(NULL);
	return (NULL);
}

/**
 * blur_image - blurs the entirety of an image using a Gaussian Blur
 * @img_blur: holds the address at which the blured image is stored
 * @img: points to the source image
 * @kernel: points to the convolution kernel to use
 */
void blur_image(img_t *img_blur, img_t const *img, kernel_t const *kernel)
{
	pthread_t threads[NUM_THREADS];
	blur_portion_t portions[NUM_THREADS];
	size_t i, slice_h, remainder_h;

	if (!img || !img_blur || !kernel)
		return;

	slice_h = img->h / NUM_THREADS;
	remainder_h = img->h % NUM_THREADS;

	for (i = 0; i < NUM_THREADS; i++)
	{
		portions[i].img = img;
		portions[i].img_blur = img_blur;
		portions[i].kernel = kernel;
		portions[i].x = 0;
		portions[i].w = img->w;
		portions[i].y = i * slice_h;

		/* Last thread handles any remaining lines */
		if (i == NUM_THREADS - 1)
			portions[i].h = slice_h + remainder_h;
		else
			portions[i].h = slice_h;

		pthread_create(&threads[i], NULL, thread_blur, &portions[i]);
	}

	for (i = 0; i < NUM_THREADS; i++)
		pthread_join(threads[i], NULL);
}
