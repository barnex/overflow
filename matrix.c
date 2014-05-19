#include "matrix.h"
#include "libovf2.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// malloc with abort on out-of-memory, init to zero
void *emalloc(size_t bytes) {
	void *ptr = malloc(bytes);
	if (ptr == NULL) {
		fprintf(stderr, "out of memory (allocating %ld bytes)\n", bytes);
		fflush(stdout);
		fflush(stderr);
		abort();
	}
	memset(ptr, 0, bytes);
	return ptr;
}

matrix newMatrix(int nx, int ny) {
	float *storage;
	storage = (float*)emalloc(nx*ny*sizeof(storage[0]));
	matrix m = {arr:storage, nx:nx, ny:ny};
	return m;
}


void abortBounds(matrix mat, int iy, int ix){
	fprintf(stderr, "matrix index [%d, %d] out of bounds [%d, %d]\n", iy, ix, mat.ny, mat.nx);
	fflush(stdout);
	fflush(stderr);
	abort();
}

// checked matrix access arr[iy][ix]
float *m(matrix mat, int iy, int ix){
	if (ix<0 || ix>=mat.nx || iy<0 || iy>=mat.ny){
		abortBounds(mat, ix, iy);
	}
	return &mat.arr[iy*mat.nx+ix];
}

void matrixMemset(matrix mat, float v){
	for(int iy=0; iy<mat.ny; iy++){
		for(int ix=0; ix<mat.nx; ix++){
			*m(mat, iy, ix) = v;
		}
	}
}

void matrixWrite(const char* fname, matrix mat){
	ovf2_data d = { valuedim: 1, xnodes: mat.nx, ynodes: mat.ny, znodes: 1, data: mat.arr};
	ovf2_writefile(fname, d);
}

