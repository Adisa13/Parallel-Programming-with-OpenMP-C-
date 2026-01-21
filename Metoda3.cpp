#include <omp.h>
#include <iostream>
#include <iomanip>
#include <stdlib.h>

using namespace std;

int main() {
    int N_values[] = { 500, 1000, 1500 };
    int total_N = 3;
    int threads_to_test[] = { 1, 2, 4, 6, 8, 12 };
    int total_T = 6;

    cout << setw(10) << "Madhesia" << setw(10) << "Threads"
         << setw(15) << "Koha (sek)" << setw(15) << "Speedup" << endl;

    for (int t = 0; t < total_N; t++) {
        int N = N_values[t];
        double *A = new double[N*N];
        double *B = new double[N*N];
        double *C = new double[N*N];

        // Inicializimi
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                A[i*N+j] = i + j + 1;
                B[i*N+j] = i - j;
                C[i*N+j] = 0;
            }
        }

        double serial_time = 0.0;

        for (int j_test = 0; j_test < total_T; j_test++) {
            int num_threads = threads_to_test[j_test];
            omp_set_num_threads(num_threads);

            // Reset C
            #pragma omp parallel for
            for (int k = 0; k < N*N; k++) C[k] = 0;

            double start_time = omp_get_wtime();

            #pragma omp parallel for schedule(static) shared(A, B, C)
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    double sum = 0.0;
                    for (int k = 0; k < N; k++) {
                        sum += A[i*N+k] * B[k*N+j];
                    }
                    C[i*N+j] = sum;
                }
            }

            double end_time = omp_get_wtime();
            double time_taken = end_time - start_time;

            if (num_threads == 1) serial_time = time_taken;
            
            double speedup = 0.0;
            if (time_taken > 0) speedup = serial_time / time_taken;

            cout << setw(10) << N
                 << setw(10) << num_threads
                 << setw(15) << fixed << setprecision(4) << time_taken
                 << setw(14) << setprecision(2) << speedup << "x" << endl;
        }
        delete[] A;
        delete[] B;
        delete[] C;
    }
    return 0;
}