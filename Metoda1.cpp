#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <iomanip>

using namespace std;

int main() {
    long long N_values[] = { 100000, 10000000, 50000000, 100000000 };
    int total_N = 4;
    int threads_to_test[] = { 1, 2, 4, 6, 12 };
    int total_T = 5;

    srand(time(0));

    cout << "STARTIMI I TESTEVE ME SPEEDUP (Analiza e Plote) " << endl;
    cout << setw(15) << "Madhesia (N)" << setw(10) << "Threads" 
         << setw(15) << "Koha (sek)" << setw(15) << "Speedup (X)" 
         << setw(15) << "Mesatarja" << endl;

    for (int i = 0; i < total_N; i++) {
        long long n = N_values[i];
        double t_serial = 0.0;

        for (int j = 0; j < total_T; j++) {
            int num_threads = threads_to_test[j];
            omp_set_num_threads(num_threads);
            
            double Sum = 0.0;
            double Aver = 0.0;
            
            double start_time = omp_get_wtime();
            
            #pragma omp parallel for reduction(+:Sum)
            for (long long k = 0; k < n; k++) {
                Sum += (double)rand() / RAND_MAX;
            }
            
            double end_time = omp_get_wtime();
            double time_taken = end_time - start_time;
            
            if (num_threads == 1) {
                t_serial = time_taken;
            }
            
            Aver = Sum / (double)n;
            double speedup = t_serial / time_taken;

            cout << setw(15) << n
                 << setw(10) << num_threads
                 << setw(15) << fixed << setprecision(5) << time_taken
                 << setw(15) << setprecision(2) << speedup
                 << setw(15) << setprecision(5) << Aver << endl;
        }
    }
    return 0;
}