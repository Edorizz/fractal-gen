/* C library */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
/* dmatrix */
#include <dmatrix.h>

#define SCR_BUF_W	640
#define SCR_BUF_H	640

#define SCR_W		640
#define SCR_H		640

#define BLACK		0
#define WHITE		1
#define MAX_COLORS	2

uint8_t colors[MAX_COLORS][3] = { {   0,   0,   0 },	/* white */
				  { 255, 255, 255 } };	/* black */

/* -2.5, 1	x
 * -1  , 1	y */

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

			if (curr_it == max_it) {
				color_pixel(scr_buf, i, j, WHITE);
			}
		}
	}
}

int
main(int argc, char **argv)
{
	struct dot_matrix dm;
	uint8_t scr_buf[SCR_BUF_H][SCR_BUF_W][3];

	dm.scr_buf = scr_buf[0][0];
	dm.scr_buf_w = SCR_BUF_W;
	dm.scr_buf_h = SCR_BUF_H;

	create_matrix(&dm, SCR_W, SCR_H);
	memset(scr_buf[0][0], 0, SCR_BUF_W * SCR_BUF_H * 3);

	mandelbrot(scr_buf, 64);
	update_matrix(&dm);

	while (!glfwWindowShouldClose(dm.win)) {
		glfwPollEvents();

		render_matrix(&dm);
		glfwSwapBuffers(dm.win);
	}

	free_matrix(&dm);
	return 0;
}
