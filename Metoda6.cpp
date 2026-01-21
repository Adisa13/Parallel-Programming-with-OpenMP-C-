#include <omp.h>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>

using namespace std;

#define n 12

// Funksioni i polinomit f(x)
double f(double x) {
    return pow(x, 12) 
         - 90 * pow(x, 11) 
         + 3641 * pow(x, 10) 
         - 87450 * pow(x, 9) 
         + 1387023 * pow(x, 8) 
         - 15282630 * pow(x, 7) 
         + 119753843 * pow(x, 6) 
         - 671189310 * pow(x, 5) 
         + 2664929476 * pow(x, 4) 
         - 7292774280 * pow(x, 3) 
         + 13020978816 * pow(x, 2) 
         - 13575738240 * x 
         + 6227020800;
}

int main() {
    int thread_counts[] = { 1, 2, 4, 6, 8, 12 };
    int num_tests = 6;
    double x[n], y[n];
    double tol = 0.5 * pow(10, -9);
    double koha_serial = 0;

    cout << setw(10) << "Threads" << setw(15) << "Koha (s)" 
         << setw(15) << "Speedup" << setw(15) << "Iteracione" << endl;

    for (int t = 0; t < num_tests; t++) {
        int num_threads = thread_counts[t];
        int i, j, it = 0;
        double P = 1, eps = 2 * tol;

        // Inicializimi i vlerave fillestare
        for (i = 0; i < n; i++) {
            x[i] = (double)i + 0.4; // Ndryshuar pak per te shmangur rrenjet ekzakte ne fillim
        }

        vector<double> gab_max(num_threads);
        omp_set_num_threads(num_threads);
        
        double start_time = omp_get_wtime();

        #pragma omp parallel shared(x, y, eps, it, gab_max) private(i, j, P)
        {
            int threadNum = omp_get_thread_num();
            int fillimi = (n * threadNum) / num_threads;
            int fundi = (n * (threadNum + 1)) / num_threads;
            double gab;

            while (eps > tol && it < 20000) {
                gab_max[threadNum] = 0;
                
                for (i = fillimi; i < fundi; i++) {
                    P = 1;
                    for (int j = 0; j < n; j++) {
                        if (i != j) {
                            P = P * (x[i] - x[j]);
                        }
                    }
                    if (abs(P) < 1e-20) P = 1e-20; // Shmangia e pjesetimit me zero
                    
                    y[i] = x[i] - f(x[i]) / P;
                    
                    gab = abs(y[i] - x[i]);
                    if (gab > gab_max[threadNum])
                        gab_max[threadNum] = gab;
                    
                    x[i] = y[i];
                }

                #pragma omp barrier

                #pragma omp master
                {
                    it++;
                    eps = 0;
                    for (int k = 0; k < num_threads; k++) {
                        if (gab_max[k] > eps) eps = gab_max[k];
                    }
                }
                #pragma omp barrier
            }
        }

        double time_taken = omp_get_wtime() - start_time;
        
        if (num_threads == 1) {
            koha_serial = time_taken;
        }
        double speedup = koha_serial / time_taken;

        cout << setw(10) << num_threads
             << setw(15) << fixed << setprecision(4) << time_taken
             << setw(14) << fixed << setprecision(2) << speedup << "x"
             << setw(15) << it << endl;
    }

    cout << "\nRrrenjet e gjetura (Zgjidhjet): " << endl;
    for (int i = 0; i < n; i++)
        cout << "x[" << i << "] = " << x[i] << endl;

    return 0;
}