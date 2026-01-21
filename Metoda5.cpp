#include <omp.h>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <stdlib.h>

using namespace std;

#define TOLERANCE 1e-9
#define MAX_ITER 5000

int main() {
    int N_values[] = { 500, 1000, 1500 };
    int total_N = 3;
    int threads_to_test[] = { 1, 2, 4, 6, 8, 12 };
    int total_T = 6;

    cout << setw(10) << "Permasa" << setw(10) << "Threads"
         << setw(15) << "Koha (sek)" << setw(15) << "Speedup"
         << setw(15) << "Iteracione" << endl;

    for (int t = 0; t < total_N; t++) {
        int N = N_values[t];
        double *A = new double[N * N];
        double *b = new double[N];
        double *x = new double[N];
        double *y = new double[N];

        // Inicializimi i matrices A dhe vektoreve
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (i == j)
                    A[i * N + j] = 2.0;
                else if (i == (j - 1) || i == (j + 1))
                    A[i * N + j] = -1.0;
                else
                    A[i * N + j] = 0.0;
            }
            b[i] = 1.0;
            x[i] = 0.0;
        }

        double serial_time = 0.0;

        for (int j_test = 0; j_test < total_T; j_test++) {
            int num_threads = threads_to_test[j_test];
            omp_set_num_threads(num_threads);
            
            // Reset x per cdo test
            for (int k = 0; k < N; k++) x[k] = 0.0;

            double start_time = omp_get_wtime();
            int iter = 0;
            double eps = TOLERANCE + 1.0;

            while (eps > TOLERANCE && iter < MAX_ITER) {
                eps = 0.0;
                
                #pragma omp parallel for reduction(max: eps) schedule(static)
                for (int i = 0; i < N; i++) {
                    double s = 0.0;
                    for (int j = 0; j < N; j++) {
                        if (i != j) {
                            s += A[i * N + j] * x[j];
                        }
                    }
                    y[i] = (b[i] - s) / A[i * N + i];
                    double v = fabs(y[i] - x[i]);
                    if (v > eps) eps = v;
                }

                #pragma omp parallel for schedule(static)
                for (int i = 0; i < N; i++) {
                    x[i] = y[i];
                }
                iter++;
            }

            double end_time = omp_get_wtime();
            double time_taken = end_time - start_time;

            if (num_threads == 1) serial_time = time_taken;
            double speedup = 0.0;
            if (time_taken > 0) speedup = serial_time / time_taken;

            cout << setw(10) << N
                 << setw(10) << num_threads
                 << setw(15) << fixed << setprecision(4) << time_taken
                 << setw(14) << setprecision(2) << speedup << "x"
                 << setw(15) << iter << endl;
        }
        delete[] A;
        delete[] b;
        delete[] x;
        delete[] y;
    }
    return 0;
}