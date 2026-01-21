#include <omp.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

using namespace std;

// Funksioni f(x) = 1/x
double f(double x) {
    return 1.0 / x;
}

int main() {
    long long N_list[] = { 10000000, 100000000, 500000000 };
    int num_N = 3;
    int threads_list[] = { 1, 2, 4, 6, 8, 12 };
    int num_threads_count = 6;
    double a = 1, b = 2;

    cout << setw(14) << "Madhesia" << setw(8) << "Threads"
         << setw(14) << "Koha (sek)" << setw(14) << "Speedup" << endl;

    for (int i = 0; i < num_N; i++) {
        long long n = N_list[i];
        double h = (b - a) / n;
        double koha_serial = 0;

        for (int j = 0; j < num_threads_count; j++) {
            int num_threads = threads_list[j];
            vector<double> sum(num_threads);
            double s = 0;

            omp_set_num_threads(num_threads);
            double start = omp_get_wtime();

            #pragma omp parallel
            {
                int id = omp_get_thread_num();
                sum[id] = 0;
                
                #pragma omp for
                for (long long k = 1; k < n; k++) {
                    sum[id] += f(a + k * h);
                }
            }

            for (int k = 0; k < num_threads; k++) s += sum[k];
            
            double result = h / 2.0 * (f(a) + 2 * s + f(b));
            
            double time_taken = omp_get_wtime() - start;
            if (time_taken < 1e-9) time_taken = 1e-9;

            double speedup = 0.0;
            if (num_threads == 1) {
                koha_serial = time_taken;
                speedup = 1.00;
            } else {
                speedup = koha_serial / time_taken;
            }

            cout << setw(14) << n
                 << setw(8) << num_threads
                 << setw(14) << fixed << setprecision(4) << time_taken
                 << setw(14) << fixed << setprecision(2) << speedup << "x" << endl;
        }
    }
    return 0;
}