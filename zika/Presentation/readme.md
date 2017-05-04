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

	-  We are trying to solve larger and larger problems, at the same time, the processors are more and more poewerful
	-  However, the processors nearly meet its limit
	-  So we are thinking use some less powerful processors and combine them together
	-  Then do the computing on the different cores and distributed memory
	-  How to make the most use of it, it is about high performance computing
2. About multiple threads
	-  Process & Threads
	-  A process creates multiple threads and these threads share memory
	-  built-in thread interface is not very good.
	-  OpenMP are designed to make the most use of the different cores.
3. About openMP:

	-  compiler directives
	-  with c, c++ and fortan
	-  easy to use
	-  take advantages of mulit-core
4. Model of openMP: Fork and Join

	If want to use openMP, first need to identity the master thread and where can be done by parallism

	Then write the parallism code

5. openMP core syntax

	-  include
	-  #pagama
	-  gcc -fopenmp file.c
	-  in fact, if we do not gcc with the paramater -fopenmp, it will be compiled as normal program. Run on platform without openMP
	-  data structure:
		- shared
		- privated: firstprivate lastprivate
		- none
	-  construct:
		- sections
		- single
		- nested
		- if/else
		- loop
	-  More details: https://computing.llnl.gov/tutorials/openMP/
6. About MPI:

	-  OpenMP is used for process with shared memory
	-  However, only one computer is not powerful enough
	-  we want combine machines together and use them to do the calculation
	-  so this is mpi, which is used to communicate between process with seperate address
	-  it is a library
	-  easy to use and learn, main for Single Program and Multiple Data or Multiple tasks and different data
7. Syntax:

	-  #include mpi.h
	-  MPI_Init($argc, $argv)/MPI_Finalize() it is different from opemnmp, which is compiler directives
	-  Compiler: mpicc file.c
	-  Run: mpirun/mpiexec -np n hello
	-  or mpirun hello on the head machine
	-  or run on cluster: mpirun --hostfile host hello host will be the file with node to run the file
8. Six basic functions

	-  init/finalize
	-  rank and size: how big our family are and who I am, so we can communicate effiectively
	-  send/rev: send and recieve message
	-  send:
		- start: address of the data to start;
		- count: number of data;
		- type of data: MPI_long, MPI_double;
		- dest: target process, rank of the process,who i want to send;
		- tag:set with tag, so receiving can identity the message, messages can be screened at the receiving end by specifying a specific tag;
		- Comm means communicator, normally is MPI_COMM_WORLD.
	-  recv:
		- start: where to receive, the start address;
		- number of block;
		- datatype;
		- source means where from, we can use MPI_ANY_SOURCE to represent message from all the nodes;
		- Tag can be speficifed which message to be received;
		- COM is the same: MPI_COMM_WORLD;
		- status is a data structure include the information : status.MPI_TAG; status.MPI_SOURCE;  
	-  processes can be collected into groups, and together we can form the groups as communicator; default communicator is MPI_COMM_WORLD
	-  the return of the functions can be mpi_success or error code
	-  block function like MPI_SEND() and MPI_RECV() won't return until the communication is finished
	-  funciton like MPI_ISend() will get return immediately.
9. Code to run:

```
		export OMPI_CC=gcc-6
	  export OMPI_CXX=g++-6

		gcc-6 -fopenmp hello1.c  
		./a.out

		gcc-6 -fopenmp hello2.c -o hello
		./hello

		gcc-6 -fopenmp hello3.c -o hello
		./hello

		mpicc mpihello1.c
		mpiexec \-n 4 a.out

		mpicc mpihello2.c
		mpiexec \-n 4 a.out
		
		mpicc -fopenmp openmpmpi.c -o openmpmpi
		mpiexec \-n 8 openmpmpi
```
