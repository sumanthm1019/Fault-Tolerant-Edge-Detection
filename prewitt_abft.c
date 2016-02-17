#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include "prewitt_abft.h"
#include<time.h>
int newx_check(int width, int height, signed char arr2[width - 2],
		signed char rowchecksumx[width - 2], signed char arr1[height - 2],
		signed char colchecksumx[height - 2], signed char newx[width][height],
		int * pcount);
int image_check(int width, int height, signed char img[width][height],
		int * pcount);
int newy_check(int width, int height, signed char arr3[width - 2],
		signed char rowchecksumy[width - 2], signed char arr4[height - 2],
		signed char colchecksumy[height - 2], signed char newy[width][height],
		int * pcount);
void calculate_checksumxy(int width, int height,
		signed char colchecksumx[height - 2],
		signed char rowchecksumx[width - 2],
		signed char colchecksumy[height - 2],
		signed char rowchecksumy[width - 2], signed char newx[width][height],
		signed char newy[width][height]);

void main() {
	long int i, j, l, k, m, n;
	int count = 0;
	int drseus_count = 0;
	signed char sumx = 0, sumy = 0;

	signed char sum, wtsum;
	long int height = 512, width = 512;
	signed char Gx[3][3], Gy[3][3];
	long int r = 0, c = 0;
	signed char newx[height][width], newy[height][width],
			prewitt[height + 1][width + 1];
	long int arrsize = height * width;
	signed char header[1078 + arrsize];
	FILE *inp, *out;
	signed char image_1dim[arrsize];
	signed char out_image_1dim[arrsize];
	signed char arr1[width - 2], arr2[height - 2], arr3[height - 2], arr4[width
			- 2];
	signed char rowsumx, colsumx, rowsumy, colsumy, sum1, sum2, mulfactor = 1;
	signed char img[height + 2][width]; //for checksum row and weighted checksum row
	signed char colchecksumx[width - 2], rowchecksumx[height - 2],
			colchecksumy[width - 2], rowchecksumy[height - 2];
	inp = fopen("lena512.bmp", "rb");
	long int index = 0;
	k = 0;
	//Input bmp image is read into 2d matrix, prior to which, header part is read into a 1d array
	for (i = 0; i < (1078 + arrsize); i++) {
		header[i] = fgetc(inp);

	}
	long int ind = 0;
	for (k = 1078; k < (1078 + arrsize); k++) {
		image_1dim[ind] = header[k];
		ind++;

	}
	ind = 0;
	for (r = 0; r < height; r++) {
		for (c = 0; c < width; c++) {
			img[r][c] = image_1dim[ind++];

		}
	}

	for (i = 0; i < width; i++) {
		wtsum = 0;
		sum = 0;
		for (j = 0; j < height; j++) {
			wtsum = wtsum + img[j][i] * pow(2, j);
			sum = sum + img[j][i];
		}
		img[j][i] = sum;
		img[j + 1][i] = wtsum;
	}

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
	//Calculation of X and Y gradient matrices
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
	// calculating row and column checksum for newx and newy matrices
	calculate_checksumxy(width, height, colchecksumx, rowchecksumx,
			colchecksumy, rowchecksumy, newx, newy);
	int ret;

	ret = image_check(width, height, img, &count) == 1);
	// using the image to calculate newx row checksum (add elements of each column)
	for (i = 1; i < width - 1; i++) {
		sum1 = 0;
		sum2 = 0;
		for (j = 0; j < height; j++) {
			if ((j == 0) || (j == height - 1))
				mulfactor = 1;
			else if ((j == 1) || (j == height - 2))
				mulfactor = 2;
			else
				mulfactor = 3;
			sum1 = sum1 + mulfactor * img[j][i + 1];

			sum2 = sum2 + mulfactor * img[j][i - 1];

		}

		arr1[i] = sum1 - sum2;

	}
	// using the image to calculate the column checksum of newx (add elements of each row)
	for (j = 1; j < height - 1; j++) {
		sum1 = 0;
		sum2 = 0;
		for (l = j - 1; l < j + 2; l++) {
			for (m = 2; m < width; m++) {
				sum1 += img[l][m];
			}
			for (n = 0; n < width - 2; n++) {
				sum2 += img[l][n];

			}
		}
		arr2[j] = sum1 - sum2;
	}

	// using the image to calculate newy column checksum (add elements of each row)
	for (i = 1; i < height - 1; i++) {
		sum1 = 0;
		sum2 = 0;
		for (j = 0; j < width; j++) {
			if ((j == 0) || (j == width - 1))
				mulfactor = 1;
			else if ((j == 1) || (j == width - 2))
				mulfactor = 2;
			else
				mulfactor = 3;
			sum1 = sum1 + mulfactor * img[i + 1][j];

			sum2 = sum2 + mulfactor * img[i - 1][j];

		}

		arr3[i] = sum1 - sum2;

	}

	//check if image is proper before checking newx and newy, if not correct image

	// using the image to calculate the row checksum of newy (add elements of each column)

	for (j = 1; j < width - 1; j++) {
		sum1 = 0;
		sum2 = 0;
		for (l = j - 1; l < j + 2; l++) {
			for (m = 2; m < height; m++) {
				sum1 += img[m][l];
			}
			for (n = 0; n < height - 2; n++) {
				sum2 += img[n][l];

			}
		}
		arr4[j] = sum1 - sum2;
	}
	//correction of X and Y gradient matrices using checksums

	ret = newx_check(width, height, arr2, rowchecksumx, arr1, colchecksumx, newx,
			&count);
	ret = newy_check(width, height, arr3, rowchecksumy, arr4, colchecksumy, newy,
			&count);

	index = 0;
	for (i = 0; i < width; i++) {
		for (j = 0; j < height; j++) {
			prewitt[i][j] = sqrt(
					(newx[i][j] * newx[i][j]) + (newy[i][j] * newy[i][j]));
			out_image_1dim[index++] = prewitt[i][j];
		}
	}

	out = fopen("prewitt_lena.bmp", "wb");
	for (i = 0; i < 1078; i++)
		fputc(header[i], out);
	fwrite(out_image_1dim, sizeof(signed char), arrsize, out);
	fclose(out);

}
