#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <iomanip>
#include <iostream>

using namespace std;

void quicksort(int a[], int ell, int elF) {
    int i, j, celes, temp;
    
    if (ell < elF) {
        celes = ell;
        i = ell;
        j = elF;
        
        while (i < j) {
            while (a[i] <= a[celes] && i < elF) i++;
            while (a[j] > a[celes]) j--;
            
            if (i < j) {
                temp = a[i];
                a[i] = a[j];
                a[j] = temp;
            }
        }
        
        temp = a[celes];
        a[celes] = a[j];
        a[j] = temp;

        // Paralelizimi me Sections per vargje te medha
        if ((elF - ell) > 1000) {
            #pragma omp parallel sections
            {
                #pragma omp section
                quicksort(a, ell, j - 1);
                
                #pragma omp section
                quicksort(a, j + 1, elF);
            }
        } else {
            quicksort(a, ell, j - 1);
            quicksort(a, j + 1, elF);
        }
    }
}

int main() {
    long N_list[] = { 100000, 500000, 1000000 };
    int threads_list[] = { 1, 2, 4, 6, 8, 12 };
    
    for (int n_idx = 0; n_idx < 3; n_idx++) {
        long current_N = N_list[n_idx];
        double koha_serial = 0;
        
        for (int t_idx = 0; t_idx < 6; t_idx++) {
            int current_threads = threads_list[t_idx];
            
            int* a = (int*)malloc(current_N * sizeof(int));
            srand(300);
            for (int i = 0; i < current_N; i++) a[i] = rand();
            
            omp_set_num_threads(current_threads);
            double fillimi = omp_get_wtime();
            
            quicksort(a, 0, current_N - 1);
            
            double ke = omp_get_wtime() - fillimi;
            
            if (current_threads == 1) koha_serial = ke;
            double speedup = koha_serial / ke;
            
            printf("%14ld %8d %14.4f %14.2fx\n", current_N, current_threads, ke, speedup);
            
            free(a);
        }
    }
    return 0;
}