#include <iostream>
#include <omp.h>
#include <cmath>
#include <iomanip>

using namespace std;

// Funksioni y = sqrt(1 - x^2)
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
        double a = 0.0, b = 1.0;
        double h = (b - a) / n;
        double t_serial = 0;

        for (int i = 0; i < num_threads_configs; i++) {
            int t = threads_list[i];
            omp_set_num_threads(t);
            
            double start = omp_get_wtime();
            double sum = 0.0;

            #pragma omp parallel for reduction(+:sum)
            for (long long j = 1; j < n; j++) {
                double x = a + j * h;
                sum += f(x);
            }

            double integral = h * (0.5 * (f(a) + f(b)) + sum);
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