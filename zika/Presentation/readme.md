# The presetation for seminar
## Outline
### Include
- Openmp
- OpenMpi
### Time line
1. About mulitple threads
2. Introduction about openmp, which is make the most use of the threads
3. The examples and how to use the openmp libraries
4. Show how to run the code, the simple one
5. Introduction about open MPI, which is to combine different machines together
6. The examples and codes
7. Simple code to run
8. Run openmp and mpi together
9. If there is enough time, show the code about cpu time, and total real time

### Content
1. Introduction about HPC
	a.  We are trying to solve larger and larger problems, at the same time, the processors are more and more poewerful
	b. However, the processors nearly meet its limit
	c. So we are thinking use some less powerful processors and combine them together
	d. Then do the computing on the different cores and distributed memory
	e. How to make the most use of it, it is about high performance computing
2. About multiple threads
	a. Process & Threads
	b. A process creates multiple threads and these threads share memory
	c. built-in thread interface is not very good. 
	d. OpenMP are designed to make the most use of the different cores.
3. About openMP:
	a. compiler directives
	b. with c, c++ and fortan
	c. easy to use
	b. take advantages of mulit-core
4. Model of openMP: Fork and Join
	If want to use openMP, first need to identity the master thread and where can be done by parallism
	Then write the parallism code
5. openMP core syntax
	a. include
	b. #pagama
	c. gcc -fopenmp file.c
	d. in fact, if we do not gcc with the paramater -fopenmp, it will be compiled as normal program. Run on platform without openMP
	e. data structure:
		- shared
		- privated: firstprivate lastprivate
		- none
	f. construct:
		- sections
		- single
		- nested
		- if/else
		- loop
	g. More details: https://computing.llnl.gov/tutorials/openMP/
6. About MPI:
	a. OpenMP is used for process with shared memory
	b. However, only one computer is not powerful enough
	c. we want combine machines together and use them to do the calculation
	d. so this is mpi, which is used to communicate between process with seperate address
	e. it is a library
	f. easy to use and learn, main for Single Program and Multiple Data or Multiple tasks and different data
7. Syntax:
	a. #include mpi.h
	b. MPI_Init($argc, $argv)/MPI_Finalize() it is different from opemnmp, which is compiler directives
	c. Compiler: mpicc file.c
	d. Run: mpirun/mpiexec -np n hello
	e. or mpirun hello on the head machine
	f. or run on cluster: mpirun --hostfile host hello host will be the file with node to run the file
8. Six basic functions
	a. init/finalize
	b. rank and size: how big our family are and who I am, so we can communicate effiectively
	c. send/rev: send and recieve message
	d. 
