#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

int newx_check(int width, int height, signed char arr2[width - 2],
		signed char rowchecksumx[width - 2], signed char arr1[height - 2],
		signed char colchecksumx[height - 2], signed char newx[width][height],
		int *pcount) {
	//checksum comparison row for newx

	int colflag = 0, colindex = 0, i, j;
	int rowflag = 0, rowindex = 0;
	int rowdiff = 0;

	for (i = 1; i < width - 1; i++) {
		if (arr2[i] != rowchecksumx[i]) {
			colflag = 1;
			rowindex = i;
			break;
		}
	}
	//checksum comparison column for newx

	for (i = 1; i < height - 1; i++) {
		if (arr1[i] != colchecksumx[i]) {
			rowflag = 1;
			colindex = i;
			rowdiff = arr1[i] - colchecksumx[i];
			break;
		}
	}

	if (colflag == 1 && rowflag == 1) {
		printf("Error due to fault in newx matrix \n");
		newx[rowindex][colindex] += rowdiff;
		return 1;
	}
	return 0;
}

int image_check(int width, int height, signed char img[][height],
		int * pcount) {

	signed char wcs1 = 0, wcs2 = 0, q = 0, z = 0, wtsum, sum;
	int i, j, wcsindex = 0;
	for (i = 0; i < width; i++) {
		wtsum = 0;
		sum = 0;
		for (j = 0; j < height; j++) {
			wtsum = wtsum + img[j][i] * pow(2, j);
			sum = sum + img[j][i];
		}
		if (sum != img[j][i]) {
			wcs1 = sum - img[j][i];
			wcsindex = i;
		}
		if (wtsum != img[j + 1][i])
			wcs2 = wtsum - img[j + 1][i];

	}
	if ((wcs1 != 0) && (wcs2 != 0)) {

		q = wcs2 / wcs1;
		z = log2(q);
		if (wcs1 > 0)
			img[z][wcsindex] -= wcs1;
		else
			img[z][wcsindex] += wcs1;
		return 1;
	}
	return 0;

}
int newy_check(int width, int height, signed char arr3[width - 2],
		signed char rowchecksumy[width - 2], signed char arr4[height - 2],
		signed char colchecksumy[height - 2], signed char newy[width][height],
		int * pcount) {
	int colflag = 0, colindex = 0, i;

	int rowflag = 0, rowindex = 0;
	signed char rowdiff = 0;
	for (i = 1; i < width - 1; i++) {
		if (arr3[i] != rowchecksumy[i]) {
			colflag = 1;
			rowindex = i;
			break;
		}
	}

	for (i = 1; i < height - 1; i++) {
		if (arr4[i] != colchecksumy[i]) {
			rowflag = 1;
			colindex = i;
			rowdiff = arr4[i] - colchecksumy[i];
			break;
		}
	}
	if (colflag == 1 && rowflag == 1) {
		printf("Error due to fault in newy matrix\n");
		newy[rowindex][colindex] += rowdiff;
		return 1;
	}

	return 0;

}
void calculate_checksumxy(int width, int height,
		signed char colchecksumx[height - 2],
		signed char rowchecksumx[width - 2],
		signed char colchecksumy[height - 2],
		signed char rowchecksumy[width - 2], signed char newx[width][height],
		signed char newy[width][height]) {
	signed char colsumx, rowsumx, colsumy, rowsumy;
	int i, j;
	for (i = 1; i < width - 1; i++) {
		colsumx = 0;
		rowsumx = 0;
		colsumy = 0;
		rowsumy = 0;
		for (j = 1; j < height - 1; j++) {
			colsumx += newx[j][i];
			rowsumx += newx[i][j];
			colsumy += newy[j][i];
			rowsumy += newy[i][j];

		}
		colchecksumx[i] = colsumx;
		rowchecksumx[i] = rowsumx;
		colchecksumy[i] = colsumy;
		rowchecksumy[i] = rowsumy;

	}

}

