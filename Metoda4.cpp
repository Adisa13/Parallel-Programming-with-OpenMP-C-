#include <stdlib.h>
#include <omp.h>
#include <iostream>
#include <math.h>
#include <iomanip>

using namespace std;

int main() {
    long long N_values[] = { 100000000, 500000000, 1000000000 };
    int total_N_tests = 3;
    int threads_to_test[] = { 1, 2, 4, 6, 8, 12 };
    int total_thread_tests = 6;

    cout << setw(15) << "Iteracione (N)" << setw(10) << "Threads"
         << setw(15) << "Koha (sek)" << setw(15) << "Speedup"
         << setw(18) << "Vlera Pi" << endl;

    for (int i = 0; i < total_N_tests; i++) {
        long long N = N_values[i];
        double serial_time = 0.0;

        for (int j = 0; j < total_thread_tests; j++) {
            int num_threads = threads_to_test[j];
            omp_set_num_threads(num_threads);
            
            double s = 0.0;
            double pi = 0.0;
            
            double start_time = omp_get_wtime();
            
            #pragma omp parallel for reduction(+:s)
            for (long long k = 0; k < N; k++) {
                double x = (double)(k + 1);
                s = s + 1.0 / (x * x);
            }
            
            pi = sqrt(6 * s);
            double end_time = omp_get_wtime();
            double time_taken = end_time - start_time;

            if (num_threads == 1) serial_time = time_taken;
            
            double speedup = 0.0;
            if (time_taken > 0) speedup = serial_time / time_taken;

            cout << setw(15) << N
                 << setw(10) << num_threads
                 << setw(15) << fixed << setprecision(4) << time_taken
                 << setw(14) << setprecision(2) << speedup << "x"
                 << setw(18) << setprecision(10) << pi << endl;
        }
    }
    return 0;
}