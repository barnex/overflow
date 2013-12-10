package overflow

import (
	"math"
)

var (
	Nx, Ny   int                //size
	KNx, KNy int                // kernel size
	cx, cy   float64     = 1, 1 // cellsize
	Rho      [][]float64        // charge density
	Kx, Ky   [][]float64        // rho->E kernel
	Ex, Ey   [][]float64        // electric field
	Sx, Sy   [][]float64        // conductivity
	Jx, Jy   [][]float64        // current density
	Bx, By   [][]float64        // boundary current density
	dt       float64     = 0.5
)

func Init(nx, ny int) {
	Nx, Ny = nx, ny

	alloc()

	// initial conductivity: 1 in bulk, 0 at boundaries
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

	initK()
}

// set staggered conductivity (Sx, Sy) from unstaggered matrix
func SetConductivity(c [][]float64) {
	for iy := range Sx {
		for ix := range Sx[iy] {
			left := getc(c, ix-1, iy)
			right := getc(c, ix, iy)
			Sx[iy][ix] = 1 / (1/left + 1/right)
		}
	}
	for iy := range Sy {
		for ix := range Sy[iy] {
			bottom := getc(c, ix, iy-1)
			top := getc(c, ix, iy)
			Sy[iy][ix] = 1 / (1/bottom + 1/top)
		}
	}
}

// get array element, or 0 for out-of-bounds
func getc(c [][]float64, ix, iy int) float64 {
	if ix == -1 || iy == -1 || ix == Nx || iy == Ny{
		return 0
	} else {
		return c[iy][ix]
	}
}

func Step() {
	updRho()
	updE()
	updj()
}

func updRho() {
	for iy := range Rho {
		for ix := range Rho[iy] {
			Rho[iy][ix] += (dt / 16) * (Jx[iy][ix] - Jx[iy][ix+1] + Jy[iy][ix] - Jy[iy+1][ix])
		}
	}
}

func updE() {

	// Zero output first
	zero(Ex)
	zero(Ey)

	for sy := range Rho {
		for Sx := range Rho[sy] {
			if Rho[sy][Sx] == 0 {
				continue // skip zero source
			}
			for y := range Ex {
				j := wrap(y-sy, KNy)
				for x := range Ex[y] {
					i := wrap(x-Sx, KNx)
					Ex[y][x] += Rho[sy][Sx] * Kx[j][i]
				}
			}
			for y := range Ey {
				j := wrap(y-sy, KNy)
				for x := range Ey[y] {
					i := wrap(x-Sx, KNx)
					Ey[y][x] += Rho[sy][Sx] * Ky[j][i]
				}
			}
		}
	}
}

func updj() {
	for iy := range Jx {
		for ix := range Jx[iy] {
			if Bx[iy][ix] == UNSET {
				Jx[iy][ix] = Sx[iy][ix] * Ex[iy][ix]
			} else {
				Jx[iy][ix] = Bx[iy][ix]
			}
		}
	}
	for iy := range Jy {
		for ix := range Jy[iy] {
			if By[iy][ix] == UNSET {
				Jy[iy][ix] = Sy[iy][ix] * Ey[iy][ix]
			} else {
				Jy[iy][ix] = By[iy][ix]
			}
		}
	}
}

func initK() {
	KNx, KNy = 2*Nx, 2*Ny
	Kx, Ky = MakeArray(KNx, KNy), MakeArray(KNx, KNy)

	for x := -Nx + 1; x <= Nx; x++ {
		xw := wrap(x, KNx)
		for y := -Ny + 1; y <= Ny; y++ {
			yw := wrap(y, KNy)

			{
				rx, ry := (float64(x)-0.5)*cx, float64(y)*cy
				r := math.Sqrt(rx*rx + ry*ry)
				r3 := r * r * r
				Ex := rx / r3
				Kx[yw][xw] = Ex
			}

			{
				rx, ry := (float64(x))*cx, (float64(y)-0.5)*cy
				r := math.Sqrt(rx*rx + ry*ry)
				r3 := r * r * r
				Ey := ry / r3
				Ky[yw][xw] = Ey
			}

		}
	}
}

func alloc() {
	Rho = MakeArray(Nx, Ny)
	Ex, Ey = staggered(Nx, Ny)
	Sx, Sy = staggered(Nx, Ny)
	Jx, Jy = staggered(Nx, Ny)
	Bx, By = staggered(Nx, Ny)

	Memset(Bx, UNSET)
	Memset(By, UNSET)
}

const UNSET = math.MaxFloat64
