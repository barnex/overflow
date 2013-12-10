package overflow

import (
	"fmt"
	"os"
)

// save array under filename
func Save(fname string, a [][]float64) {
	f, err := os.OpenFile(fname, os.O_CREATE|os.O_TRUNC|os.O_WRONLY, 0666)
	check(err)
	defer f.Close()

	for iy := range a {
		for ix := range a[iy] {
			fmt.Fprint(f, a[iy][ix], " ")
		}
		fmt.Fprintln(f)
	}
}

// unstagger and save
func Save2D(fname string, ax, ay [][]float64) {
	f, err := os.OpenFile(fname, os.O_CREATE|os.O_TRUNC|os.O_WRONLY, 0666)
	check(err)
	defer f.Close()

	Nx := len(ay[0]) // not staggered in x
	Ny := len(ax)    // not staggered in y

	for y := 0; y < Ny; y++ {
		for x := 0; x < Nx; x++ {
			valx := 0.5 * (ax[y][x] + ax[y][x+1])
			valy := 0.5 * (ay[y][x] + ay[y+1][x])
			fmt.Fprintln(f, x, y, valx, valy)
		}
		fmt.Fprintln(f)
	}
}
