#include "matrix.h"
#include <stdio.h>

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
