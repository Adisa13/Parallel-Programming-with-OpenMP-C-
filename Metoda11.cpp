#include <iostream>
#include <cmath>
#include <omp.h>
#include <iomanip>

using namespace std;

int main() {
    long long n_values[] = { 1000000, 10000000, 50000000 };
    int num_experiments = 3;
    int threads_list[] = { 1, 2, 4, 6, 8, 12 };
    int num_threads_configs = 6;
    double x = 0.99999999;

    cout << left << setw(12) << "N"
         << setw(10) << "Threads"
         << setw(14) << "Koha (s)"
         << setw(14) << "Speedup (X)"
         << setw(18) << "Efikasiteti (%)"
         << "Vlera e Polinomit" << endl;

    for (int exp = 0; exp < num_experiments; exp++) {
        long long N = n_values[exp];
        
        // Alokim dinamik per shkak te madhesise se madhe
        double* A = new double[N];
        double* P_x = new double[N];
        
        double t_serial = 0;

        for (int i = 0; i < num_threads_configs; i++) {
            int t = threads_list[i];
            omp_set_num_threads(t);
            
            double start = omp_get_wtime();

            // 1. Inicializimi i koeficienteve
            #pragma omp parallel for
            for (long long j = 0; j < N; j++) {
                A[j] = 1.000001;
            }

            // 2. Llogaritja e fuqive
            #pragma omp parallel for
            for (long long j = 0; j < N; j++) {
                P_x[j] = pow(x, j);
            }

            // 3. Prodhimi Skalar (Reduction)
            double vlera_polinomit = 0.0;
            #pragma omp parallel for reduction(+:vlera_polinomit)
            for (long long j = 0; j < N; j++) {
                vlera_polinomit += A[j] * P_x[j];
            }

            double end = omp_get_wtime();
            double time_taken = end - start;
            if (time_taken < 1e-9) time_taken = 1e-9;

            if (t == 1) t_serial = time_taken;
            
            double speedup = t_serial / time_taken;
            double efficiency = (speedup / t) * 100.0;

            cout << left << setw(12) << N
                 << setw(10) << t
                 << fixed << setprecision(5) << setw(14) << time_taken
                 << fixed << setprecision(2) << setw(14) << speedup
                 << fixed << setprecision(2) << setw(18) << efficiency
                 << scientific << setprecision(5) << vlera_polinomit << endl;
        }
        delete[] A;
        delete[] P_x;
    }
    return 0;
}