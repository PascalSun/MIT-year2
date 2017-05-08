# Notes
## What to setup
- IDE: Xcode
- Libraries:
	- openmp
	- mpi
	- opencl
- compiler: cmake
## About gcc
```
alias gcc-6 gcc
export OMPI_CC=gcc-6
export OMPI_CXX=g++-6
```
## To make the hello.c code run

`mpicc -framework opencl -fopenmp hello.c`
