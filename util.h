//DOBRA Nicoleta-Emilia, 311CA
#ifndef UTIL_H
#define UTIL_H

//pixel value struct r,g,b for ppm, x for pgm
typedef struct {
	int r, g, b;
	int x;
} pixel;

pixel **alloc_mat(int n, int m);

int load(FILE **f, pixel ***mat, int *n, int *m);

void free_mat(pixel **mat, int n);

void save_file(pixel **mat, int n, int m, int type);

void select_pixels(int *x1, int *y1, int *x2, int *y2, int n, int m, int type);

void crop_image(pixel ***mat, int *x1, int *y1,
				int *x2, int *y2, int *n, int *m, int type);

void apply_image(pixel **mat, int x1, int y1,
				 int x2, int y2, int type, int n, int m);

void equalize(pixel **mat, int n, int m, int type);

void histogram(pixel **mat, int n, int m, int type);

void rotate_image(pixel ***mat, int *x1, int *y1, int *x2, int *y2,
				  int *n, int *m, int type);

#endif
