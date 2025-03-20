//DOBRA Nicoleta-Emilia, 311CA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "util.h"

//function to swap 2 integers
void swap_int(int *a, int *b)
{
	int aux = *a;
	*a = *b;
	*b = aux;
}

//function that returns a dynamically allocated pixel matrix
pixel **alloc_mat(int n, int m)
{
	pixel **a = malloc(n * sizeof(pixel *));
	if (!a) {
		printf("Error at malloc!");
		return NULL;
	}
	for (int i = 0; i < n; i++) {
		a[i] = malloc(m * sizeof(pixel));
		if (!a[i]) {
			printf("Error at malloc!");
			for (int j = 0; j < i; j++) {
				free(a[j]);
			}
			free(a);
			return NULL;
		}
	}
	return a;
}

//function that frees a dynamically allocated pixel matrix
void free_mat(pixel **mat, int n)
{
	if (!mat) {
		return;
	}
	for (int i = 0; i < n; i++) {
		free(mat[i]);
	}
	free(mat);
	mat = NULL;
}

//extracts the header in a pgm/ppm image
void extract_header(FILE *f, int *n, int *m, int *type)
{
	char ch;
	int k = 0;
	while ((ch = fgetc(f)) != EOF) {
		//if the first character in a line is # then the line is a comment
		if (ch == '#') {
			while ((ch = fgetc(f)) != EOF && ch != '\n') {

			}
		} else {
			//otherwise move the pointer one step back and read the line
			fseek(f, -1, SEEK_CUR);
			k++;
			if (k == 1) {
				char s[3];
				fscanf(f, "%2s", s);
				*type = s[1] - '0';
			} else if (k == 2)
				fscanf(f, "%d%d", m, n);
			else if (k == 3) {
				int value;
				fscanf(f, "%d", &value);
			} else
				break;
		}
	}
}

//function for the load command
//loads image and extracts data
int load(FILE **f, pixel ***mat, int *n, int *m)
{
	char file_name[100];
	int type;
	//read file name and open file
	scanf("%s", file_name);
	*f = fopen(file_name, "r");
	if (!(*f)) {
		printf("Failed to load %s\n", file_name);
		*mat = NULL;
		return -1;
	}
	//read header
	extract_header(*f, n, m, &type);
	*mat = alloc_mat(*n, *m);
	if (!(*mat)) {
		printf("Failed to load %s\n", file_name);
		fclose(*f);
		return -1;
	}
	//read pixel matrix
	if (type == 2 || type == 3) {
		for (int i = 0; i < *n; i++) {
			for (int j = 0; j < *m; j++) {
				if (type == 2)
					fscanf(*f, "%d", &(*mat)[i][j].x);
				else
					fscanf(*f, "%d %d %d",
						   &(*mat)[i][j].r, &(*mat)[i][j].g, &(*mat)[i][j].b);
			}
		}
	} else {
		fgetc(*f);
		for (int i = 0; i < *n; i++) {
			if (type == 5) {
				//read pgm image
				unsigned char *buffer = malloc(*m * sizeof(unsigned char));
				if (!buffer) {
					printf("Failed to load %s\n", file_name);
					fclose(*f);
					return -1;
				}
				fread(buffer, sizeof(unsigned char), *m, *f);
				for (int j = 0; j < *m; j++)
					(*mat)[i][j].x = buffer[j];
				free(buffer);
			} else {
				//read ppm image (r, g, b values)
				unsigned char *buffer = malloc(*m * 3 * sizeof(unsigned char));
				if (!buffer) {
					printf("Failed to load %s\n", file_name);
					fclose(*f);
					return -1;
				}
				fread(buffer, sizeof(unsigned char), *m * 3, *f);
				for (int j = 0; j < *m; j++) {
					(*mat)[i][j].r = buffer[3 * j];
					(*mat)[i][j].g = buffer[3 * j + 1];
					(*mat)[i][j].b = buffer[3 * j + 2];
				}
				free(buffer);
			}
		}
	}
	//close file and print message
	fclose(*f);
	printf("Loaded %s\n", file_name);
	return type;
}

//function that saves data to file
void save_file(pixel **mat, int n, int m, int type)
{
	char file_name[100], mode[10];
	scanf("%s", file_name);
	scanf("%c", &mode[0]);
	if (mode[0] != '\n') {
		//saved file must be text
		char ch = mode[0];
		int k = 1;
		while (ch != '\n') {
			scanf("%c", &ch);
			mode[k++] = ch;
		}
		if (type == -1) {
			printf("No image loaded\n");
			return;
		}
		FILE *output = fopen(file_name, "wt");
		if (!output) {
			printf("Error at opening file\n");
			return;
		}
		//print data to file
		if (type == 2 || type == 5) {
			fprintf(output, "P2\n");
		} else {
			fprintf(output, "P3\n");
		}
		fprintf(output, "%d %d\n", m, n);
		fprintf(output, "255\n");
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				if (type == 2 || type == 5) {
					fprintf(output, "%d ", mat[i][j].x);
				} else {
					fprintf(output, "%d %d %d ",
							mat[i][j].r, mat[i][j].g, mat[i][j].b);
				}
			}
			fprintf(output, "\n");
		}
		fclose(output);
	} else {
		//saved file must be binary
		if (type == -1) {
			printf("No image loaded\n");
			return;
		}
		FILE *output = fopen(file_name, "wb");
		if (!output) {
			printf("Error at opening file\n");
			return;
		}
		//print data to file
		if (type == 2 || type == 5) {
			fprintf(output, "P5\n");
		} else {
			fprintf(output, "P6\n");
		}
		fprintf(output, "%d %d\n", m, n);
		fprintf(output, "255\n");
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				if (type == 2 || type == 5) {
					char ch = mat[i][j].x;
					fwrite(&ch, sizeof(char), 1, output);
				} else {
					char ch = mat[i][j].r;
					fwrite(&ch, sizeof(char), 1, output);
					ch = mat[i][j].g;
					fwrite(&ch, sizeof(char), 1, output);
					ch = mat[i][j].b;
					fwrite(&ch, sizeof(char), 1, output);
				}
			}
		}
		fclose(output);
	}
	printf("Saved %s\n", file_name);
}

//function checks whether selected pixels are valid
int is_valid(int x1, int y1, int x2, int y2, int n, int m)
{
	if (x1 == x2 || y1 == y2) {
		return 0;
	}
	if (x1 >= 0 && x1 <= n &&
		y1 >= 0 && y1 <= m &&
		x2 >= 0 && x2 <= n &&
		y2 >= 0 && y2 <= m) {
		return 1;
	}
	return 0;
}

/*function reads selection and arranges pixels
  so that (x1,y1) - upper left, (x2,y2) - lower right
*/
void select_pixels(int *x1, int *y1, int *x2, int *y2, int n, int m, int type)
{
	char mode[100], ch = '\0';
	int k = 0;
	//read the rest of the characters till the end of the line
	scanf("%c", &ch);
	while (ch != '\n') {
		scanf("%c", &ch);
		mode[k++] = ch;
	}
	mode[k - 1] = '\0';
	int a1, b1, a2, b2;
	if (strcmp(mode, "ALL") == 0 || strcmp(mode, "ALL ") == 0) {
		//if the command is SELECT ALL (x1,y1)=(0,0) and (x2,y2)=(n,m)
		if (type == -1) {
			printf("No image loaded\n");
			return;
		}
		*x1 = 0;
		*y1 = 0;
		*x2 = n;
		*y2 = m;
		printf("Selected ALL\n");
	} else if (sscanf(mode, "%d %d %d %d", &b1, &a1, &b2, &a2) == 4) {
		//if the command is SELECT y1 x1 y2 x2
		if (type == -1) {
			printf("No image loaded\n");
			return;
		}
		if (is_valid(a1, b1, a2, b2, n, m)) {
			//arrange x1,y1,x2,y2
			*x1 = a1;
			*y1 = b1;
			*x2 = a2;
			*y2 = b2;
			if (*x1 > *x2) {
				swap_int(x1, x2);
			}
			if (*y1 > *y2) {
				swap_int(y1, y2);
			}
			printf("Selected %d %d %d %d\n", *y1, *x1, *y2, *x2);
		} else {
			printf("Invalid set of coordinates\n");
		}
	} else {
		printf("Invalid command\n");
	}
}

//function that crops image to the selected region
void crop_image(pixel ***mat, int *x1, int *y1, int *x2, int *y2,
				int *n, int *m, int type)
{
	if (type == -1) {
		printf("No image loaded\n");
		return;
	}
	//allocate memory to the selected dimension
	pixel **aux = alloc_mat(*x2 - *x1, *y2 - *y1);
	if (!aux) {
		return;
	}
	//populate new matrix with the proper values from the original matrix
	for (int i = *x1; i < *x2; i++) {
		for (int j = *y1; j < *y2; j++) {
			aux[i - *x1][j - *y1] = (*mat)[i][j];
		}
	}
	free_mat(*mat, *n);
	*mat = aux;
	//update selection and dimension of matrix
	*n = *x2 - *x1;
	*m = *y2 - *y1;
	*x1 = 0;
	*y1 = 0;
	*x2 = *n;
	*y2 = *m;
	printf("Image cropped\n");
}

//clamp function to keep pixel values in bounds
int clamp(int value)
{
	if (value < 0)
		return 0;
	if (value > 255)
		return 255;
	return value;
}

//apply image kernel to matrix
void apply_effect(pixel **mat, int kernel[3][3],
				  int x1, int y1, int x2, int y2, int n, int m, int d)
{
	//create duplicate matrix
	pixel **aux = alloc_mat(n, m);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			aux[i][j] = mat[i][j];
		}
	}
	for (int i = x1; i < x2; i++) {
		for (int j = y1; j < y2; j++) {
			//check if pixel is out of bounds
			if (i == 0 || j == 0 || i == n - 1 || j == m - 1) {
				continue;
			}
			int sum_r = 0, sum_g = 0, sum_b = 0;
			//apply kernel to pixel and neighbours
			for (int x = -1; x <= 1; x++) {
				for (int y = -1; y <= 1; y++) {
					sum_r += aux[i + x][j + y].r * kernel[1 + x][1 + y];
					sum_g += aux[i + x][j + y].g * kernel[1 + x][1 + y];
					sum_b += aux[i + x][j + y].b * kernel[1 + x][1 + y];
				}
			}
			//update pixel in original matrix
			mat[i][j].r = clamp(sum_r / d);
			mat[i][j].g = clamp(sum_g / d);
			mat[i][j].b = clamp(sum_b / d);
		}
	}
	free_mat(aux, n);
}

//apply command function
void apply_image(pixel **mat, int x1, int y1, int x2, int y2,
				 int type, int n, int m)
{
	//read command parameter
	char mode[100], ch;
	scanf("%c", &ch);
	if (ch == '\n') {
		if (type == -1) {
			printf("No image loaded\n");
			return;
		}
		printf("Invalid command\n");
		return;
	}
	int k = 0;
	while (ch != '\n') {
		scanf("%c", &ch);
		mode[k++] = ch;
	}
	mode[k - 1] = '\0';
	if (type == -1) {
		printf("No image loaded\n");
		return;
	}
	if (type == 2 || type == 5) {
		printf("Easy, Charlie Chaplin\n");
		return;
	}
	//apply effect on matrix depending on parameter
	if (strcmp(mode, "EDGE") == 0) {
		int kernel[3][3] = {{-1, -1, -1}, {-1, 8, -1}, {-1, -1, -1}};
		apply_effect(mat, kernel, x1, y1, x2, y2, n, m, 1);
	} else if (strcmp(mode, "SHARPEN") == 0) {
		int kernel[3][3] = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};
		apply_effect(mat, kernel, x1, y1, x2, y2, n, m, 1);
	} else if (strcmp(mode, "BLUR") == 0) {
		int kernel[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
		apply_effect(mat, kernel, x1, y1, x2, y2, n, m, 9);
	} else if (strcmp(mode, "GAUSSIAN_BLUR") == 0) {
		int kernel[3][3] = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};
		apply_effect(mat, kernel, x1, y1, x2, y2, n, m, 16);
	} else {
		printf("APPLY parameter invalid\n");
		return;
	}
	printf("APPLY %s done\n", mode);
}

//equalize function
void equalize(pixel **mat, int n, int m, int type)
{
	if (type == -1) {
		printf("No image loaded\n");
		return;
	}
	if (type == 3 || type == 6) {
		printf("Black and white image needed\n");
		return;
	}

	//count number of appearences of pixel values in matrix
	long long vf[256] = {0};
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			vf[mat[i][j].x]++;
		}
	}

	double area = n * m;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			double sum = 0;
			for (int l = 0; l <= mat[i][j].x; l++) {
				sum += vf[l];
			}
			//equalization formula
			double new_value = round(255.0 * sum / area);
			//update matrix value
			mat[i][j].x = clamp(new_value);
		}
	}
	printf("Equalize done\n");
}

//histogram function
void histogram(pixel **mat, int n, int m, int type)
{
	//x - max stars and y - bins
	int x, y;
	//read command parameters
	char mode[100], buffer[100], ch = '\0';
	int k = 0;
	scanf("%c", &ch);
	while (ch != '\n') {
		scanf("%c", &ch);
		mode[k++] = ch;
	}
	if (k > 0)
		mode[k - 1] = '\0';
	else
		mode[0] = '\0';
	int ok = sscanf(mode, "%d%d%s", &x, &y, buffer);
	//check if comand is valid
	if (type == -1) {
		printf("No image loaded\n");
		return;
	}
	if (ok != 2) {
		printf("Invalid command\n");
		return;
	}
	if (type == 3 || type == 6) {
		printf("Black and white image needed\n");
		return;
	}
	//count number of appearances of bin values in matrix
	long long vf[256] = {0}, max_app = 0;
	int div = 256 / y;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			vf[mat[i][j].x / div]++;
		}
	}
	//determine max value of appearances in matrix
	for (int i = 0; i < y; i++) {
		if (vf[i] > max_app) {
			max_app = vf[i];
		}
	}
	//display histogram
	for (int i = 0; i < y; i++) {
		long long stars = vf[i] * x / max_app;
		printf("%lld\t|\t", stars);
		for (int j = 1; j <= stars; j++) {
			printf("*");
		}
		printf("\n");
	}
}

//checks if selection is square
int is_square(int x1, int y1, int x2, int y2)
{
	return (x2 - x1 == y2 - y1);
}

//check if selection is whole matrix
int is_full(int x1, int y1, int x2, int y2, int n, int m)
{
	return (x1 == 0 && y1 == 0 && x2 == n && y2 == m);
}

//function that rotates matrix 180 degrees
void rotate_180(pixel **mat, int x1, int y1, int x2, int y2, int type)
{
	// Traverse top and bottom rows
	for (int i = x1, j = x2 - 1; i <= j; i++, j--) {
		for (int l = y1, k = y2 - 1; l < y2; l++, k--) {
			if (i == j) {
				// Handle the center row for odd row count
				if (l >= k)
					break;
			}
			if (type == 2 || type == 5) {
				swap_int(&mat[i][l].x, &mat[j][k].x);
			} else {
				swap_int(&mat[i][l].r, &mat[j][k].r);
				swap_int(&mat[i][l].g, &mat[j][k].g);
				swap_int(&mat[i][l].b, &mat[j][k].b);
			}
		}
	}
}

//function that rotates matrix 270 degrees
void rotate_270(pixel ***mat, int x1, int y1, int x2, int y2, int full)
{
	//create auxiliary matrix and populate with rotated selection
	pixel **aux = alloc_mat(y2 - y1, x2 - x1);
	for (int i = y2 - 1; i >= y1; i--) {
		for (int j = x1; j < x2; j++) {
			aux[y2 - 1 - i][j - x1] = (*mat)[j][i];
		}
	}

	if (full == 0) {
		//if selection is not the full matrix update matrix values
		for (int i = x1; i < x2; i++) {
			for (int j = y1; j < y2; j++) {
				(*mat)[i][j] = aux[i - x1][j - y1];
			}
		}
		free_mat(aux, y2 - y1);
	} else {
		//if selection is full matrix free original matrix and asign pointer
		free_mat(*mat, x2 - x1);
		*mat = aux;
	}
}

//function that rotates matrix 270 degrees
void rotate_90(pixel ***mat, int x1, int y1, int x2, int y2, int full)
{
	//create auxiliary matrix and populate with rotated selection
	pixel **aux = alloc_mat(y2 - y1, x2 - x1);
	for (int i = y1; i < y2; i++) {
		for (int j = x2 - 1; j >= x1; j--) {
			aux[i - y1][x2 - 1 - j] = (*mat)[j][i];
		}
	}

	if (full == 0) {
		//if selection is not the full matrix update matrix values
		for (int i = x1; i < x2; i++) {
			for (int j = y1; j < y2; j++) {
				(*mat)[i][j] = aux[i - x1][j - y1];
			}
		}
		free_mat(aux, y2 - y1);
	} else {
		//if selection is full matrix free original matrix and asign pointer
		free_mat(*mat, x2 - x1);
		*mat = aux;
	}

}

//rotate function
void rotate_image(pixel ***mat, int *x1, int *y1, int *x2, int *y2,
				  int *n, int *m, int type)
{
	//read command parameter
	char mode[100], ch = '\0';
	int k = 0;
	scanf("%c", &ch);
	while (ch != '\n') {
		scanf("%c", &ch);
		mode[k++] = ch;
	}
	if (k > 0)
		mode[k - 1] = '\0';
	else
		mode[0] = '\0';
	//check if command is valid
	if (type == -1) {
		printf("No image loaded\n");
		return;
	}
	int angle, ok;
	ok = sscanf(mode, "%d", &angle);
	if (ok != 1) {
		printf("Invalid command\n");
		return;
	}
	if (angle % 90 != 0) {
		printf("Unsupported rotation angle\n");
		return;
	}
	if (!is_square(*x1, *y1, *x2, *y2) &&
		!is_full(*x1, *y1, *x2, *y2, *n, *m)) {
		printf("The selection must be square\n");
		return;
	}
	//rotate function depending on angle
	if (angle == 270 || angle == -90) {
		if (is_full(*x1, *y1, *x2, *y2, *n, *m)) {
			rotate_270(mat, *x1, *y1, *x2, *y2, 1);
			//update selection and bounds
			swap_int(x2, y2);
			swap_int(n, m);
		} else {
			rotate_270(mat, *x1, *y1, *x2, *y2, 0);
		}

	} else if (angle == 180 || angle == -180) {
		rotate_180(*mat, *x1, *y1, *x2, *y2, type);
	} else if (angle == 90 || angle == -270) {
		if (is_full(*x1, *y1, *x2, *y2, *n, *m)) {
			rotate_90(mat, *x1, *y1, *x2, *y2, 1);
			//updte selection and bounds
			swap_int(x2, y2);
			swap_int(n, m);
		} else {
			rotate_90(mat, *x1, *y1, *x2, *y2, 0);
		}
	}
	printf("Rotated %d\n", angle);
}
