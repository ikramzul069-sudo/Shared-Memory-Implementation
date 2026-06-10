# HPC Infection Spread Simulation: Serial vs Shared Memory (OpenMP)

<div align="center">
  <img src="https://img.shields.io/badge/Language-C-blue.svg" alt="Language C">
  <img src="https://img.shields.io/badge/Parallel-OpenMP-orange.svg" alt="OpenMP">
</div>

---

## 1. About This Project & File Structure

### Project Description
This project is a high-performance simulation modeling the spread of an infection or disease within a 2D grid population using the C programming language. 

The primary objective of this project is to demonstrate, evaluate, and compare the scalability and computational performance between two architectural approaches:
* **Sequential (Serial Approach):** Standard baseline execution utilizing a single thread.
* **Shared Memory (Parallel Approach):** Parallel execution utilizing **OpenMP** to distribute the computational workload across multiple threads within a single node or multi-core processor.

This project covers essential High-Performance Computing (HPC) concepts, including thread-level parallelism, shared memory management, loop parallelization, I/O efficiency, and preventing race conditions.

### File Structure & Functionality
Here is a breakdown of what each file in this repository does:
* **`main_serial.c`** : Contains the core simulation logic executed sequentially (as a baseline) on a single CPU core.
* **`main_shared.c`** : Contains the optimized simulation logic using the OpenMP library to run in parallel across a shared memory environment.
* **`file_reader.c`** : A helper module designed to efficiently read, parse, and allocate population matrix data from external files.
* **`simulation.h`** : A header file containing shared function prototypes, data structures, and state definitions used across the project.
* **`Makefile`** : A build automation script to compile the C source code into ready-to-use executables without needing to type long manual compiler commands.
* **`pop_*.dat` (e.g., `pop_128_128.dat`)** : Input dataset files containing the initial population matrix (e.g., 128x128 dimensions) to be simulated.

---

## 2. Set-Up
Before running this project, ensure your local environment has the following dependencies installed:

1. **C Compiler:** GCC (a modern version supporting the C99 standard).
2. **OpenMP Support:** Built into most modern GCC installations (requires the `-fopenmp` flag, which is already configured in the Makefile).
3. **Build Tools:** The `make` utility to execute the Makefile.

## 3. Compilation
Open your terminal in the project directory and use the Makefile to compile both the serial and parallel versions simultaneously:
```bash
make
```
This command will generate two executables:
gccserial (for the sequential version)
gccshared (for the parallel OpenMP version)

## 4. Execution
Both programs require 4 input arguments in the following order: [Infection Radius] [Recovery Time] [Max Runs] [Input File].

Running the Serial Version:

```Bash
./gccserial 4 3 1000 pop_128_128.dat
```

Running the Parallel (OpenMP) Version:
You can specify the number of threads you want to use by exporting the OMP_NUM_THREADS environment variable before executing the program (Example using 4 threads):
```Bash
export OMP_NUM_THREADS=4
./gccshared 4 3 1000 pop_128_128.dat
```

Running on an HPC Cluster (Slurm):
Submit the job script to the cluster's queue using the following command:
```Bash
sbatch slurm_simulation.sh
```
