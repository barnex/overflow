// +build ignore

package main

import (
	. "."
	"fmt"
)

func main() {
	Init(32, 16)

	s := MakeArray(Nx, Ny)
	Memset(s, 1)
	s[Nx/4][Ny/2] = 0
	s[Nx/4-1][Ny/2] = 0
	s[Nx/4][Ny/2-1] = 0
	s[Nx/4-1][Ny/2-1] = 0
	SetConductivity(s)

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
