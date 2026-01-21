# Parallel Programming and Performance Analysis with OpenMP (C++)

This repository contains the implementation and empirical performance analysis of 14 numerical and sorting algorithms parallelized using **C++** and the **OpenMP** .The project was developed as part of a Master's level laboratory in Mathematical and Computer Engineering.

## 💻 Hardware Environment
All benchmarks were conducted on the following system:
* **CPU:** AMD Ryzen 5 PRO 7540U w/ Radeon 740M Graphics.
* **Cores:** 6 Physical Cores.
* **Logical Processors:** 12 Threads.
* **Memory:** 16 GB RAM.
* **OS:** Windows 11 Pro, Version 24H2.

## 🧪 Implemented Methods
The project covers a wide range of computational problems:
1.  **Parallel Average Calculation:** Using `reduction` for large datasets.
2.  **Vector Summation:** Optimized with dynamic scheduling.
3.  **Matrix Multiplication:** Parallelized $O(N^3)$ complexity.
4.  **Pi Approximation:** Euler series implementation.
5.  **Jacobi Iterative Method:** Solving linear systems.
6.  **Durand-Kerner Method:** Simultaneous root-finding for polynomials.
7.  **Trapezoidal Rule:** Parallel numerical integration.
8.  **Simpson's Composite Rule:** High-precision integration.
9.  **Parallel QuickSort:** Task-based parallelization using `sections`.
10. **Bubble Sort (Odd-Even):** Parallel sorting via index-based phases.
11. **Factorial Calculation ($n!$):** Synthetic load benchmarking.
12. **Polynomial Evaluation:** Dot product approach for high-degree polynomials.
13. **Power Method:** Finding dominant eigenvalues.
14. **Parallel Bisection (Multisection):** Accelerated root-finding.

## 📊 Key Results & Analysis
The project evaluates parallel performance using **Speedup** and **Efficiency** metrics.

| Category | Finding |
| :--- | :--- |
| **Highest Speedup** | **Parallel Bisection** achieved a **55x speedup** by reducing iterations from 1000 (serial) to 7 (parallel). |
| **Optimal Scaling** | **Matrix Multiplication** showed high scalability, reaching **8.7x speedup** with 12 threads. |
| **Parallel Overhead** | **For small problems like **Durand-Kerner ($N=12$)**, serial execution was faster due to thread management costs. |
| **Memory Bound** | **Polynomial Evaluation** was limited by memory bandwidth, reaching a maximum speedup of **6.79x**. |
| **False Sharing** | **Initial integration tests identified performance drops due to cache contention, later optimized using private variables. |

## 📁 Repository Structure
* `/Source_Codes`: C++ files for each numerical method.
* `Programim_paralel_Report.pdf`: Full laboratory report in Albanian.

## 🎓 Author
**Adisa Hoxhaj** MSc Student in Mathematical and Computer Engineering  
University of Tirana, Faculty of Natural Science
