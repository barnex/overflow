#include "libovf2.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	float *arr;
	int nx;
	int ny;
} matrix;

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

static int Nx=0, Ny=0; // size
static matrix Rho;     // charge density
static matrix Ex, Ey;  // electric field
static matrix Sx, Sy;  // conductivity
static matrix Jx, Jy;  // current density
static matrix Bx, By;  // boundary current density


void alloc() {
	Rho = newMatrix(Nx, Ny);                             
	Ex  = newMatrix(Nx+1, Ny); Ey = newMatrix(Nx, Ny+1); 
	Sx  = newMatrix(Nx+1, Ny); Sy = newMatrix(Nx, Ny+1); 
	Jx  = newMatrix(Nx+1, Ny); Jy = newMatrix(Nx, Ny+1); 
	Bx  = newMatrix(Nx+1, Ny); By = newMatrix(Nx, Ny+1); 
}

int main() {
	Nx = 32;
	Ny = 16;

	printf("size: %dx%d\n", Nx, Ny);

	alloc();

	// default conductivity: 1 in bulk, 0 at boundaries
	matrixMemset(Sx, 1);
	matrixMemset(Sy, 1);
	for (int ix=0; ix<Sy.nx; ix++){
		*m(Sy, 0,       ix) = 0;
		*m(Sy, Sy.ny-1, ix) = 0;
	}
	for (int iy=0; iy<Sx.ny; iy++){
		*m(Sx, iy,       0) = 0;
		*m(Sx, iy, Sx.nx-1) = 0;
	}

	matrixWrite("Sx.ovf", Sx);
	matrixWrite("Sy.ovf", Sy);

	return 0;
}
