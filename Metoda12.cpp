#include <iostream>
#include <vector>
#include <cmath>
#include <omp.h>
#include <iomanip>

using namespace std;

// Funksion qe gjeneron elementin A[i][j]
inline double get_A(int i, int j) {
    if (i == j) return i + 1.0;
    return 1.0 / (abs(i - j) + 1.0);
}

int main() {
    int n_values[] = { 5000, 10000, 20000 };
    int num_experiments = 3;
    int threads_list[] = { 1, 2, 4, 6, 8, 12 };
    int num_threads_configs = 6;
    
    const int max_iter = 20;
    const double tol = 1e-6;

    cout << left << setw(10) << "N"
         << setw(10) << "Threads"
         << setw(14) << "Koha (s)"
         << setw(14) << "Speedup (X)"
         << "Efikasiteti (%)" << endl;

    for (int exp = 0; exp < num_experiments; exp++) {
        int n = n_values[exp];
        vector<double> x(n, 1.0);
        vector<double> y(n);
        
        double t_serial = 0;
        double final_lambda = 0;

        for (int t_idx = 0; t_idx < num_threads_configs; t_idx++) {
            int num_threads = threads_list[t_idx];
            omp_set_num_threads(num_threads);
            
            // Reset vector x
            fill(x.begin(), x.end(), 1.0);
            
            double lambda_new = 0;
            double start_time = omp_get_wtime();

            for (int iter = 0; iter < max_iter; iter++) {
                
                // Hapi 1: Shumëzimi Matrice-Vektor
                #pragma omp parallel for schedule(static)
                for (int i = 0; i < n; i++) {
                    double sum = 0.0;
                    for (int j = 0; j < n; j++) {
                        sum += get_A(i, j) * x[j];
                    }
                    y[i] = sum;
                }

                // Hapi 2: Gjetja e maksimumit (Norma)
                double max_val = 0.0;
                #pragma omp parallel for reduction(max:max_val)
                for (int i = 0; i < n; i++) {
                    double val = fabs(y[i]);
                    if (val > max_val) max_val = val;
                }
                lambda_new = max_val;

                // Hapi 3: Normalizimi
                #pragma omp parallel for schedule(static)
                for (int i = 0; i < n; i++) {
                    x[i] = y[i] / lambda_new;
                }
            }

            double end_time = omp_get_wtime();
            double time_taken = end_time - start_time;
            if (time_taken < 1e-9) time_taken = 1e-9;

            if (num_threads == 1) t_serial = time_taken;
            
            double speedup = t_serial / time_taken;
            double efficiency = (speedup / num_threads) * 100.0;
            final_lambda = lambda_new;

            cout << left << setw(10) << n
                 << setw(10) << num_threads
                 << fixed << setprecision(4) << setw(14) << time_taken
                 << fixed << setprecision(2) << setw(14) << speedup
                 << fixed << setprecision(2) << efficiency << endl;
        }
        cout << "-> Rezultati (Lambda) per N=" << n << " ishte: " 
             << fixed << setprecision(5) << final_lambda << endl;
    }
    return 0;
}