#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	double *arr;
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
	double *storage;
	storage = (double*)emalloc(nx*ny*sizeof(storage[0]));
	matrix m = {arr:storage, nx:nx, ny:ny};
	return m;
}


void abortBounds(matrix mat, int iy, int ix){
	fprintf(stderr, "matrix index [%d, %d] out of bounds [%d, %d]\n", iy, ix, mat.ny, mat.nx);
	fflush(stdout);
	fflush(stderr);
	abort();
}

// checked matrix access
double *m(matrix mat, int iy, int ix){
	if (ix<0 || ix>=mat.nx || iy<0 || iy>=mat.ny){
		abortBounds(mat, ix, iy);
	}
	return &mat.arr[ix*mat.ny+iy];
}

void matrixMemset(matrix mat, double v){
	for(int iy=0; iy<mat.ny; iy++){
		for(int ix=0; ix<mat.nx; ix++){
			*m(mat, iy, ix) = v;
		}
	}
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

	return 0;
}
