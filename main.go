package main

import(
	"math"
	"os"
	"fmt"
)

var (
	Nx, Ny int         //size
	cx, cy float64 = 1, 1 // cellsize
	rho    [][]float64 // charge density
	Kx, Ky [][]float64 // rho->E kernel
	Ex, Ey [][]float64 // electric field
	sx, sy [][]float64 // conductivity
	jx, jy [][]float64 // current density
	Bx, By [][]float64 // boundary current density
	dt float64
)

func main() {
	Nx, Ny = 32, 16

	alloc()

	initK()
	save("Kx", Kx)
	save("Ky", Ky)
}


func updRho(){
	for iy := range rho{
		for ix := range rho[iy]{
			rho[iy][ix] += dt * (jx[iy][ix] + jx[iy][ix+1] + jy[iy][ix] + jy[iy+1][ix])
		}
	}
}

func updE(){
	
}

func initK(){
	KNx, KNy := 2*Nx, 2*Ny
	Kx, Ky = arr2d(KNx, KNy), arr2d(KNx, KNy)

	for x := -Nx+1; x <= Nx; x++{
		xw := wrap(x, KNx)
		for y := -Ny+1; y <= Ny; y++{
			yw := wrap(y, KNy)

			{
			rx, ry := (float64(x) - 0.5) * cx, float64(y) * cy
			r := math.Sqrt(rx*rx + ry*ry)
			r3 := r*r*r
			Ex := rx / r3
			Kx[yw][xw] = Ex
			}

			{
			rx, ry := (float64(x)) * cx, (float64(y)-0.5) * cy
			r := math.Sqrt(rx*rx + ry*ry)
			r3 := r*r*r
			Ey := ry / r3
			Ky[yw][xw] = Ey
			}

		}
	}
}

func alloc(){
	rho = arr2d(Nx, Ny)
	Ex, Ey = staggered(Nx, Ny)
	sx, sy = staggered(Nx, Ny)
	jx, jy = staggered(Nx, Ny)
	Bx, By = staggered(Nx, Ny)
}

func staggered(Nx, Ny int) (x [][]float64, y[][]float64){
	return arr2d(Nx+1, Ny), arr2d(Nx, Ny+1)
}

func arr2d(Nx, Ny int) [][]float64 {
	list := make([]float64, Nx*Ny)
	arr := make([][]float64, Ny)
	for iy := 0; iy < Ny; iy++ {
		arr[iy] = list[iy*Nx : (iy+1)*Nx]
	}
	assert(len(arr) == Ny)
	assert(len(arr[0]) == Nx)
	return arr
}

func wrap(number, max int) int {
	for number < 0 {
		number += max
	}
	for number >= max {
		number -= max
	}
	return number
} 

func save(fname string, a[][]float64){
	f, err := os.OpenFile(fname, os.O_CREATE | os.O_TRUNC | os.O_WRONLY, 0666)
	check(err)
	defer f.Close()

	for iy := range a{
		for ix := range a[iy]{
			fmt.Fprint(f, a[iy][ix], " ")
		}
		fmt.Fprintln(f)
	}
}

func check (err error){
	if err != nil{
		panic(err)
	}
}

func assert(test bool){
	if !test{
		panic("assertion failed")
	}
}
