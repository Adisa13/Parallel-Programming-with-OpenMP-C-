#include <iostream>
#include <omp.h>
#include <iomanip>
#include <vector>

using namespace std;

#define LOAD_LOOP 20000

int main() {
    int n_values[] = { 100, 1000, 10000, 100000 };
    int thread_counts[] = { 1, 2, 3, 4 };

    cout << left << setw(8) << "N"
         << setw(10) << "Threads"
         << setw(15) << "Koha (s)"
         << setw(12) << "Speedup"
         << setw(15) << "Efikasiteti"
         << "Masa (%)" << endl;

    for (int n : n_values) {
        double t_serial = 0;
        
        for (int p : thread_counts) {
            omp_set_num_threads(p);
            double start_time = omp_get_wtime();
            
            long double fact = 1.0;
            
            // Cikel sintetike per te rritur ngarkesen
            for (int k = 0; k < LOAD_LOOP; k++) {
                fact = 1.0;
                #pragma omp parallel for reduction(*:fact)
                for (int j = 1; j <= n; j++) {
                    fact *= j; // Kujdes: fact del jashte limitit shpejt, por qellimi eshte matja e kohes
                }
            }
            
            double end_time = omp_get_wtime();
            double time_taken = end_time - start_time;
            
            if (p == 1) {
                t_serial = time_taken;
            }
            
            double speedup = t_serial / time_taken;
            double efficiency = speedup / p;
            double efficiency_percent = efficiency * 100.0;
            
            cout << left << setw(8) << n
                 << setw(10) << p
                 << fixed << setprecision(6) << setw(15) << time_taken
                 << setprecision(2) << setw(12) << speedup
                 << setw(15) << efficiency
                 << setprecision(2) << efficiency_percent << "%" << endl;
        }
    }
    return 0;
}