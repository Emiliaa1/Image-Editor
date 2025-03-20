//DOBRA Nicoleta-Emilia, 311CA
#include <stdio.h>
#include <string.h>
#include "util.h"

int main(void)
{
	FILE *file = NULL;
	pixel **mat = NULL;
	// n, [x1,x2) - height; m, [y1,y2) - width
	int x1, y1, x2, y2, n = 0, m = 0, type = -1;

	while (1) {
		char s[100];
		//read commands and call functions
		scanf("%s", s);
		if (strcmp(s, "LOAD") == 0) {
			if (mat) {
				free_mat(mat, n);
			}
			type = load(&file, &mat, &n, &m);
			//initially selection is matrix itself
			x1 = 0;
			y1 = 0;
			x2 = n;
			y2 = m;
		} else if (strcmp(s, "SAVE") == 0) {
			save_file(mat, n, m, type);
		} else if (strcmp(s, "EXIT") == 0) {
			if (type == -1) {
				printf("No image loaded\n");
			}
			break;
		} else if (strcmp(s, "SELECT") == 0) {
			select_pixels(&x1, &y1, &x2, &y2, n, m, type);
		} else if (strcmp(s, "CROP") == 0) {
			crop_image(&mat, &x1, &y1, &x2, &y2, &n, &m, type);
		} else if (strcmp(s, "APPLY") == 0) {
			apply_image(mat, x1, y1, x2, y2, type, n, m);
		} else if (strcmp(s, "EQUALIZE") == 0) {
			equalize(mat, n, m, type);
		} else if (strcmp(s, "ROTATE") == 0) {
			rotate_image(&mat, &x1, &y1, &x2, &y2, &n, &m, type);
		} else if (strcmp(s, "HISTOGRAM") == 0) {
			histogram(mat, n, m, type);
		} else {
			char ch = '\0';
			while (ch != '\n') {
				scanf("%c", &ch);
			}
			printf("Invalid command\n");
		}
	}
	if (mat) {
		free_mat(mat, n);
	}
	return 0;
}
