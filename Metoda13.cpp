#include <iostream>
#include <omp.h>
#include <cmath>
#include <iomanip>

using namespace std;

// FUNKSIONI: Shuma e nje Serie Fourier (Sinjal Valor)
double f_real(double x) {
    double sum = 0.0;
    // Mbledhim 1000 harmonika te vales (ngarkese mesatare reale)
    for (int k = 1; k <= 1000; k++) {
        sum += sin(k * x) / k;
    }
    return sum - 0.5; // Kerkojme ku sinjali ka vleren 0.5
}

int main() {
    int threads_list[] = { 1, 2, 4, 6, 8, 12 };
    int num_configs = 6;
    
    // Toleranca standarde inxhinierike
    const double tol = 1e-7;
    const int max_iter = 1000;

    cout << left << setw(10) << "Threads"
         << setw(14) << "Koha (s)"
         << setw(14) << "Speedup (X)"
         << setw(14) << "Iteracione"
         << "Rrenja" << endl;

    double t_serial = 0;

    for (int t_idx = 0; t_idx < num_configs; t_idx++) {
        int num_threads = threads_list[t_idx];
        
        // Intervali ku dime qe ka rrenje per kete funksion
        double a = 0.1;
        double b = 3.0;
        double eps = 1.0;
        int iter = 0;

        omp_set_num_threads(num_threads);
        double start_time = omp_get_wtime();

        while (eps > tol && iter < max_iter) {
            iter++;
            double h = (b - a) / num_threads;
            double current_a = a;
            bool found = false;

            #pragma omp parallel
            {
                // Optimizim: Nese u gjet, threads te tjere ndalojne
                if (!found) {
                    int id = omp_get_thread_num();
                    
                    // Ndarja e intervalit ne N pjesë
                    double x0 = current_a + id * h;
                    double x1 = x0 + h;

                    // Llogaritja e funksionit real
                    double fx0 = f_real(x0);
                    double fx1 = f_real(x1);

                    // Kontrolli i Bolzanos
                    if (fx0 * fx1 < 0) {
                        #pragma omp critical
                        {
                            if (!found) { // Vetem i pari fiton te drejten te ndryshoje a,b
                                a = x0;
                                b = x1;
                                found = true;
                            }
                        }
                    }
                }
            }
            eps = b - a;
        }

        double root = (a + b) / 2.0;
        double time_taken = omp_get_wtime() - start_time;
        
        if (time_taken < 1e-9) time_taken = 1e-9;
        if (num_threads == 1) t_serial = time_taken;
        
        double speedup = t_serial / time_taken;

        cout << left << setw(10) << num_threads
             << fixed << setprecision(5) << setw(14) << time_taken
             << fixed << setprecision(2) << setw(14) << speedup
             << setw(14) << iter
             << fixed << setprecision(5) << root << endl;
    }
    return 0;
}