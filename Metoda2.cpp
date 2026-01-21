#include <omp.h>
#include <iostream>
#include <iomanip>

#define CHUNKSIZE 1000
using namespace std;

int main() {
    long long N_values[] = { 10000000, 50000000, 100000000 };
    int total_N_tests = 3;
    int threads_to_test[] = { 1, 2, 4, 6, 8, 12 };
    int total_thread_tests = 6;
    int chunk = CHUNKSIZE;

    cout << " TESTIMI I VEKTOREVE (Schedule Dynamic) AUTOMATIK" << endl;
    cout << setw(15) << "Madhesia (N)" << setw(10) << "Threads"
         << setw(15) << "Koha (sek) " << setw(15) << "Speedup" << endl;

    for (int i = 0; i < total_N_tests; i++) {
        long long N = N_values[i];
        float *a = new float[N];
        float *b = new float[N];
        float *c = new float[N];

        // Inicializimi sekuencial (nuk matet koha)
        for (long long k = 0; k < N; k++) {
            a[k] = k * 1.0f;
            b[k] = k * 2.0f;
        }

        double serial_time = 0.0;

        for (int j = 0; j < total_thread_tests; j++) {
            int num_threads = threads_to_test[j];
            omp_set_num_threads(num_threads);
            
            double start_time = omp_get_wtime();

            #pragma omp parallel shared(a,b,c,chunk)
            {
                #pragma omp for schedule(dynamic, chunk) nowait
                for (long long k = 0; k < N; k++) {
                    c[k] = a[k] + b[k];
                }
            }

            double end_time = omp_get_wtime();
            double time_taken = end_time - start_time;

            if (num_threads == 1) {
                serial_time = time_taken;
            }

            double speedup = 0.0;
            if (time_taken > 0.0000001) speedup = serial_time / time_taken;

            cout << setw(15) << N
                 << setw(10) << num_threads
                 << setw(15) << fixed << setprecision(5) << time_taken
                 << setw(14) << setprecision(2) << speedup << "x" << endl;
        }
        delete[] a;
        delete[] b;
        delete[] c;
    }
    return 0;
}