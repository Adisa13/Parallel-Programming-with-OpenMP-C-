#include <iostream>
#include <omp.h>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <ctime>

using namespace std;

void bubble_sort_odd_even(vector<int>& arr, int n, int num_threads) {
    omp_set_num_threads(num_threads);
    
    for (int i = 0; i < n; ++i) {
        
        // FAZA 1: Indekset Teke (1, 3, 5...)
        #pragma omp parallel for
        for (int j = 1; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }

        // FAZA 2: Indekset Çifte (0, 2, 4...)
        #pragma omp parallel for
        for (int j = 0; j < n - 1; j += 2) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

int main() {
    int N_values[] = { 10000, 30000, 50000 };
    int num_N_tests = 3;
    int threads_list[] = { 1, 2, 4, 6, 8, 12 };
    int num_configs = 6;

    srand(time(0));

    for (int n_idx = 0; n_idx < num_N_tests; n_idx++) {
        int N = N_values[n_idx];
        
        cout << "--------- TESTIMI ME N = " << N << " ELEMENTE ---------\n";
        cout << left << setw(10) << "Threads"
             << setw(15) << "Koha (s)"
             << setw(15) << "Speedup (X)" << endl;

        // Gjenerojmë të dhënat vetëm një herë për këtë N
        vector<int> data(N);
        for(int i = 0; i < N; i++) data[i] = rand() % 100000;

        double t_serial = 0;

        for(int t_idx = 0; t_idx < num_configs; t_idx++) {
            int t = threads_list[t_idx];
            
            // Krijojmë një kopje të freskët për cdo test
            vector<int> test_arr = data;

            double start = omp_get_wtime();
            bubble_sort_odd_even(test_arr, N, t);
            double end = omp_get_wtime();

            double duration = end - start;
            if (t == 1) t_serial = duration;
            
            double speedup = t_serial / duration;

            cout << left << setw(10) << t
                 << fixed << setprecision(4) << setw(15) << duration
                 << fixed << setprecision(2) << setw(15) << speedup << endl;
        }
        cout << "\n";
    }
    return 0;
}