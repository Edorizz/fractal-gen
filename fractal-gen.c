/* C library */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
/* dmatrix */
#include <dmatrix.h>

#define SCR_BUF_W	1280
#define SCR_BUF_H	720

#define SCR_W		1280
#define SCR_H		720

#define BLACK		0
#define WHITE		1
#define MAX_COLORS	2

#define MAX_IT		64

uint8_t colors[MAX_COLORS][3] = { {   0,   0,   0 },	/* white */
				  { 255, 255, 255 } };	/* black */

int
_atoi(const char *str)
{
	int n;

	for (n = 0; *str; ++str) {
		n = n * 10 + *str - '0';
	}

	return n;
}

void
color_pixel(uint8_t scr_buf[][SCR_BUF_W][3], int py, int px, int color)
{
	if (color >= 0 && color < MAX_COLORS) {
		memcpy(scr_buf[py][px], colors[color], 3);
	}
}

void
mandelbrot(uint8_t scr_buf[][SCR_BUF_W][3], int max_it)
{
	double rx, iy, sx, sy, tmp;
	int i, j, curr_it;

	memset(scr_buf[0][0], 0, SCR_BUF_W * SCR_BUF_H * 3);
	for (i = 0; i != SCR_BUF_H; ++i) {
		for (j = 0; j != SCR_BUF_W; ++j) {
			curr_it = 0;
			sx = 3.5 / SCR_BUF_W * j - 2.5;
			sy = 2.0 / SCR_BUF_H * i - 1.0;
			rx = iy = 0.0;

			for (curr_it = 0; curr_it != max_it && rx * rx + iy * iy < 4; ++curr_it) {
				tmp = rx * rx - iy * iy + sx;
				iy = 2 * rx * iy + sy;
				rx = tmp;
			}

			if (curr_it < max_it) {
				scr_buf[i][j][0] = (double) curr_it / (double) max_it * 255;
				scr_buf[i][j][1] = (double) curr_it / (double) max_it * 255;

			} else {
				color_pixel(scr_buf, i, j, BLACK);
			}
		}
	}
}

void
cmplx_mult(double *rx, double *iy, double rx2, double iy2)
{
	double tmp;

	tmp = *rx * rx2 - *iy * iy2;
	*iy = *rx * iy2 + *iy * rx2;
	*rx = tmp;
}

void
fractal_gen(uint8_t scr_buf[][SCR_BUF_W][3], int exp, int max_it)
{
	int i, j, k, curr_it;
	double sx, sy, rx, iy;

	for (i = 0; i != SCR_BUF_H; ++i) {
		for (j = 0; j != SCR_BUF_W; ++j) {
			iy = sy = 2.0 / SCR_BUF_H * i - 1.0;
			rx = sx = 3.5 / SCR_BUF_W * j - 2.5;

			for (curr_it = 0; rx * rx + iy * iy < 4.0 && curr_it != max_it; ++curr_it) {
				for (k = 1; k != exp; ++k) {
					cmplx_mult(&rx, &iy, rx, iy);
				}

				rx += sx;
				iy += sy;
			}

			if (curr_it < max_it) {
				scr_buf[i][j][0] = (double) curr_it / (double) max_it * 255;
				scr_buf[i][j][1] = (double) curr_it / (double) max_it * 255;

			} else {
				color_pixel(scr_buf, i, j, BLACK);
			}
		}
	}
}

int
main(int argc, char **argv)
{
	struct dot_matrix dm;
	uint8_t scr_buf[SCR_BUF_H][SCR_BUF_W][3];
	int max_it, exp, i;

	max_it = MAX_IT;
	/* Default to Mandelbrot set */
	exp = 2;

	for (i = 1; i != argc; ++i) {
		fprintf(stderr, "Reading arg %s\n", argv[i]);
		if (argv[i][0] == '-') {
			switch (argv[i][1]) {
			case 'i':
				max_it = _atoi(argv[++i]);
				break;

			case 'e':
				exp = _atoi(argv[++i]);
				break;

			default:
				fprintf(stderr, "Error: Expected option after '-'; skipping arguments\n");
				break;
			}
		}
	}


	dm.scr_buf = scr_buf[0][0];
	dm.scr_buf_w = SCR_BUF_W;
	dm.scr_buf_h = SCR_BUF_H;

	create_matrix(&dm, SCR_W, SCR_H);
	memset(scr_buf[0][0], 0, SCR_BUF_W * SCR_BUF_H * 3);

	fractal_gen(scr_buf, exp, max_it);
	update_matrix(&dm);

	while (!glfwWindowShouldClose(dm.win)) {
		glfwPollEvents();

		render_matrix(&dm);
		glfwSwapBuffers(dm.win);
	}

	free_matrix(&dm);
	return 0;
}
