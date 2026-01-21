#include <iostream>
#include <omp.h>
#include <cmath>
#include <iomanip>

using namespace std;

// Funksioni matematikor: y = sqrt(1-x^2)
double f(double x) {
    return sqrt(1.0 - x * x);
}

int main() {
    long long n_values[] = { 10000000, 100000000, 500000000 };
    int num_experiments = 3;
    int threads_list[] = { 1, 2, 4, 6, 8, 12 };
    int num_threads_configs = 6;

    cout << left << setw(12) << "N"
         << setw(10) << "Threads"
         << setw(14) << "Koha (s)"
         << setw(14) << "Speedup (X)"
         << setw(18) << "Efikasiteti (%)"
         << "PI e Llogaritur" << endl;

    for (int exp = 0; exp < num_experiments; exp++) {
        long long n = n_values[exp];
        
        // Metoda e Simpsonit kërkon N çift.
        if (n % 2 != 0) n++;

        double a = 0.0, b = 1.0;
        double h = (b - a) / n;
        double t_serial = 0;

        for (int i = 0; i < num_threads_configs; i++) {
            int t = threads_list[i];
            omp_set_num_threads(t);
            
            double start = omp_get_wtime();
            double sum_odd = 0.0;  // Koeficienti 4
            double sum_even = 0.0; // Koeficienti 2

            #pragma omp parallel
            {
                // FAZA 1: Indekset TEKE (1, 3, 5...)
                // 'nowait' lejon threads të kalojnë te FAZA 2 pa pritur të tjerët
                #pragma omp for reduction(+:sum_odd) nowait
                for (long long j = 1; j < n; j += 2) {
                    sum_odd += f(a + j * h);
                }

                // FAZA 2: Indekset ÇIFT (2, 4, 6...)
                #pragma omp for reduction(+:sum_even)
                for (long long j = 2; j < n; j += 2) {
                    sum_even += f(a + j * h);
                }
            }

            double integral = (h / 3.0) * (f(a) + f(b) + 4.0 * sum_odd + 2.0 * sum_even);
            double pi_calc = 4.0 * integral;
            
            double end = omp_get_wtime();
            double time_taken = end - start;
            if (time_taken < 1e-9) time_taken = 1e-9;

            if (t == 1) t_serial = time_taken;
            
            double speedup = t_serial / time_taken;
            double efficiency = (speedup / t) * 100.0;

            cout << left << setw(12) << n
                 << setw(10) << t
                 << fixed << setprecision(5) << setw(14) << time_taken
                 << fixed << setprecision(2) << setw(14) << speedup
                 << fixed << setprecision(2) << setw(18) << efficiency
                 << fixed << setprecision(10) << pi_calc << endl;
        }
    }
    return 0;
}