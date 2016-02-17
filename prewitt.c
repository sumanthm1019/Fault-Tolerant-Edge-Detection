#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

void main() {
	long int i, j, l, k, m, n;
	int count = 0;
	signed char sumx = 0, sumy = 0;

	long int height = 512, width = 512;
	long int r = 0, c = 0;
	signed char newx[height][width], newy[height][width],
			prewitt[height][width];
	int Gx[3][3], Gy[3][3];
	long int offset = 1078;
	long int size = height * width;
	signed char header[offset + size];
	FILE *inp, *out;
	signed char image_1dim[size];
	long int index = 0;
	signed char img[height][width]; //for checksum row and weighted checksum row
	signed char out_image_1dim[size];
	inp = fopen("lena512.bmp", "rb");

	//Input bmp image is read into 2d matrix, prior to which, header part is read into a 1d array
	printf("Header: %ld\n ", size);
	for (i = 0; i < (offset + size); i++) {
		header[i] = fgetc(inp);

	}

	long int ind = 0;
	for (k = 1078; k < (offset + size); k++) {
		image_1dim[ind] = header[k];

		ind++;

	}
	printf("index - %ld", ind);

	ind = 0;
	for (r = 0; r < height; r++) {
		for (c = 0; c < width; c++) {
			img[r][c] = image_1dim[ind++];

		}
	}

	index = 0;
	//initialise all matrices
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			newx[i][j] = img[i][j];
			newy[i][j] = img[i][j];
			prewitt[i][j] = img[i][j];
			out_image_1dim[index++] = img[i][j];
		}
	}

	//defining the prewitt filter
	Gx[0][0] = -1;
	Gx[0][1] = 0;
	Gx[0][2] = 1;
	Gx[1][0] = -1;
	Gx[1][1] = 0;
	Gx[1][2] = 1;
	Gx[2][0] = -1;
	Gx[2][1] = 0;
	Gx[2][2] = 1;

	Gy[0][0] = -1;
	Gy[0][1] = -1;
	Gy[0][2] = -1;
	Gy[1][0] = 0;
	Gy[1][1] = 0;
	Gy[1][2] = 0;
	Gy[2][0] = 1;
	Gy[2][1] = 1;
	Gy[2][2] = 1;

	//Calculation of X gradient and Y gradient matrices
	for (i = 0; i < height - 2; i++) {
		for (j = 0; j < width - 2; j++) {
			sumx = 0;
			sumy = 0;
			r = 0;
			for (k = i; k < 3 + i; k++) {
				c = 0;
				for (l = j; l < 3 + j; l++) {

					sumx += Gx[r][c] * img[k][l];
					sumy += Gy[r][c] * img[k][l];
					c++;
				}
				r++;
			}

			newx[k - 2][l - 2] = sumx;
			newy[k - 2][l - 2] = sumy;

		}
	}
	printf("\n");

	index = 0;

	//calculation of edge detected image matrix using X and Y gradients
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			prewitt[i][j] = sqrt(
					(newx[i][j] * newx[i][j]) + (newy[i][j] * newy[i][j]));
			//prewitt[i][j] = newx[i][j] + newy[i][j];
			out_image_1dim[index++] = prewitt[i][j];

		}

	}

	//Output image is written into bmp file
	out = fopen("prewitt_lena.bmp", "wb");
	for (i = 0; i < offset; i++)
		fputc(header[i], out);
	fwrite(out_image_1dim, sizeof(signed char), size, out);

	fclose(out);

}

