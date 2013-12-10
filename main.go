package main

var(
	Nx, Ny int
	jx, jy [][]float64
)

func main(){
	Nx = 100
	Ny = 50

	jx = arr2d(Nx + 1, Ny)
	jy = arr2d(Nx, Ny + 1)
}

func arr2d(Nx, Ny int)[][]float64{
	list := make([]float64, Nx*Ny)
	arr := make([][]float64, Ny)
	for iy := 0; iy < Ny; iy ++{
		arr[iy] = list[iy*Nx: (iy+1)*Nx]
	}
	return arr
}
