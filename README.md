C implementation of an infection spread simulation in a 2D grid. Features a performance comparison between serial and shared memory parallel processing via OpenMP.

# Infection Spread Simulation: Serial vs OpenMP (Shared Memory)

## Project Overview
This project simulates the spread of an infection/disease within a 2D grid population using the C programming language. The primary goal of this repository is to demonstrate and compare the performance differences between a standard **Sequential (Serial)** execution and a **Parallel** execution utilizing Shared Memory via **OpenMP**.

This project showcases fundamental concepts of high-performance computing (HPC), state management in simulations, and memory-efficient C programming.

## Tech Stack & Features
* **Language:** C
* **Parallel Computing:** OpenMP (`<omp.h>`)
* **Build System:** `make` (Makefile)
* **Cluster Deployment:** Slurm Workload Manager (`.sh` script)
* **Features:** * High-performance custom random number generator (`xorshift32`).
  * Modular codebase separating I/O operations from core simulation logic.
  * Scalable to handle varying population grid sizes.

## Repository Structure
* `main_serial.c` : Core simulation logic executed in a single thread.
* `main_shared.c` : Core simulation logic optimized with OpenMP for parallel execution.
* `file_reader.c` & `simulation.h` : Helper modules for handling dataset parsing and shared structures.
* `pop_5_5.dat`, `pop_20_20.dat`, `pop_128_128.dat` : Input datasets representing different grid sizes.
* `Makefile` : Build automation script to compile the executables easily.
* `slurm_simulation.sh` : Batch script to run the simulation on an HPC cluster using Slurm.

## How to Build and Run

## 1. Compilation
Make sure you have GCC and OpenMP installed. You can compile both serial and shared-memory versions simultaneously using the provided Makefile:
```bash
make
The simulation requires 4 arguments: [Radius (R)] [Recovery Time] [Max Runs] [Input File]
./serial_sim 2 5 100 pop_20_20.dat
Running the Parallel (OpenMP) version:
./shared_sim 2 5 100 pop_20_20.dat

If you are deploying this on an HPC environment, submit the batch script:
sbatch slurm_simulation.sh

Expected Results:
For a 128x128 grid running 1000 iterations, Radius 4, and recorvery time 3, the OpenMP version reduced execution time by 85.95% compared to the serial version, utilizing 8 threads.

