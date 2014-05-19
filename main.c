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

	return 0;
}
