# Prototype for Zika Virus Modelling Project
## Basic Information
- Translate original python code to c++
- Add openMP

## Libraries needed
* CMake 3.2 or later required
* Boost 1.57.0 or later
* python-numpy needed if plotting wanted

It recommended that you use msys2 on Windows, brew on OSX, pacman or apt-get on linux and g++ compiler in order to utilize the package manager.

### Load libraries on Magnus
Compiling over Magnus is different than compile it over local machine. This apply to every supercomputers having multiple compile environment, module management and SLURM workload management.

To enable compile it, make sure you have the following module loaded:
- CMake/3.4.1
- Boost/1.57.0
- GNU Program Environment (PrgEnv-gnu)
- gcc/4.9.2

This can be achieve by module load / swap, by running the following command:
- `module swap PrgEnv-cray PrgEnv-gnu`
- `module swap gcc gcc/4.9.2`
- `module load cmake/3.4.1`
- `module load boost/1.57.0`  
When finished executing these commands from your logon terminal, you are ready to run `cmake`. after `cmake` finished, you can run `make` to compile the code.

### Linux Machine
Sometimes, Magnus will do some management with the libraries, so will have some problems to make it run sometimes
We have set up a linux machine which can be used to run the prototype
  - ip: 45.76.117.185
  - password: 2!kBF1Y!q?[@Hs9p

## How to run it
### Load Libraries
  - login Magnus
  - Load libraries as above
### Cmake
  - upload it to  Magnus, and `cd ./Epidemica`
  - `mkdir build `
  - `cd ./build`
  - `cmake ../`

### Compile
  - `make `
### Run
  - `cd ./bin`
  - Simple Test Run: `./VirusSimulator`
  - Simple Test Run with openMP: `./VirusSimulator --implementation=OMP OMP_NUM_THREADS=[number of threads you want to use]`
  - Run with different test data: `./VirusSimulator --scenario=[absolute path to scenario file or related path on your working directory]`
  - Run it with plotting: `./VirusSimulator --epicurve`

#### About Test Data
  - the `actual` file include the actual data Joel sent to us
  - the `523` file include data generated randomly with large scenarios
  - the other are some basic test data

## How to clone it for Magnus
  - Run `ssh-keygen ` in the terminal, and just press `Enter` to finish it
  - Start agent: `ssh-agent /bin/bash`
  - add: `ssh-add ~/.ssh/id_rsa`
  - `ssh-add -l `
  - show the ssh-key: `cat ~/.ssh/id_rsa.pub`
  - Copy and Paste it in the setting of bitbucket or github
  - Reference: https://confluence.atlassian.com/bitbucket/set-up-ssh-for-git-728138079.html
