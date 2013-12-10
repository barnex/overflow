package overflow

func staggered(Nx, Ny int) (x [][]float64, y [][]float64) {
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

func Memset(a [][]float64, v float64) {
	for iy := range a {
		for ix := range a[iy] {
			a[iy][ix] = v
		}
	}
}

func zero(a [][]float64) {
	Memset(a, 0)
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

func check(err error) {
	if err != nil {
		panic(err)
	}
}

func assert(test bool) {
	if !test {
		panic("assertion failed")
	}
}
