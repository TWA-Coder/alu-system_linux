#include "multithreading.h"

/**
 * blur_pixel - Calculates the blurred value of a single pixel
 * @portion: Information about the blur
 * @x: X coordinate of the pixel
 * @y: Y coordinate of the pixel
 */
static void blur_pixel(blur_portion_t const *portion, size_t x, size_t y)
{
	float r = 0, g = 0, b = 0, weight_sum = 0, weight;
	long half = portion->kernel->size / 2;
	long img_x, img_y, k_x, k_y;
	size_t idx;

	for (k_y = 0; k_y < (long)portion->kernel->size; k_y++)
	{
		for (k_x = 0; k_x < (long)portion->kernel->size; k_x++)
		{
			img_x = x + k_x - half;
			img_y = y + k_y - half;

			if (img_x >= 0 && img_x < (long)portion->img->w &&
			    img_y >= 0 && img_y < (long)portion->img->h)
			{
				weight = portion->kernel->matrix[k_y][k_x];
				idx = img_y * portion->img->w + img_x;
				r += portion->img->pixels[idx].r * weight;
				g += portion->img->pixels[idx].g * weight;
				b += portion->img->pixels[idx].b * weight;
				weight_sum += weight;
			}
		}
	}

	idx = y * portion->img->w + x;
	portion->img_blur->pixels[idx].r = r / weight_sum;
	portion->img_blur->pixels[idx].g = g / weight_sum;
	portion->img_blur->pixels[idx].b = b / weight_sum;
}

/**
 * blur_portion - blurs a portion of an image using a Gaussian Blur
 * @portion: pointer to the portion structure
 */
void blur_portion(blur_portion_t const *portion)
{
	size_t x, y;

	if (!portion || !portion->img || !portion->img_blur || !portion->kernel)
		return;

	for (y = portion->y; y < portion->y + portion->h; y++)
	{
		for (x = portion->x; x < portion->x + portion->w; x++)
		{
			blur_pixel(portion, x, y);
		}
	}
}
