#ifndef _MATRIX_H_
#define _MATRIX_H_

typedef struct {
	float *arr;
	int nx;
	int ny;
} matrix;


matrix newMatrix(int nx, int ny);

float *m(matrix mat, int iy, int ix);

void matrixMemset(matrix mat, float v);

void matrixWrite(const char* fname, matrix mat);

#endif
