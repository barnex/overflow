// +build ignore

package main

import (
	. "."
	"fmt"
)

func main() {
	Nx, Ny = 32, 16

	Save("Kx", Kx)
	Save("Ky", Ky)

	Memset(Sx, 1)
	Memset(Sy, 1)
	for ix := range Sy[0] {
		Sy[0][ix] = 0
		Sy[len(Sy)-1][ix] = 0
	}
	for iy := range Sx {
		Sx[iy][0] = 0
		Sx[iy][len(Sx[iy])-1] = 0
	}
	Save("sx", Sx)
	Save("sy", Sy)

	Bx[Ny/2][0] = 1
	Bx[Ny/2-1][0] = 1

	By[0][Nx/2] = -1
	By[0][Nx/2-1] = -1

	for i := 0; i < 1000; i++ {
		if i%10 == 0 {
			Save(fmt.Sprintf("%v%06d", "rho", i), Rho)
			Save(fmt.Sprintf("%v%06d", "Ex", i), Ex)
			Save(fmt.Sprintf("%v%06d", "Ey", i), Ey)
			Save(fmt.Sprintf("%v%06d", "jx", i), Jx)
			Save(fmt.Sprintf("%v%06d", "jy", i), Jy)
			Save2D(fmt.Sprintf("%v%06d", "j", i), Jx, Jy)
		}
		Step()
	}

}
