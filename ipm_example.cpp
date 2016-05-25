#include <cstdio>
#include <cstring>
#include <vector>
#include <time.h>

#include <divsufsort.h>

#include "esa.hpp"
#include "ipm.hpp"

int main(int argc, char** argv) {

    FILE *f = fopen(argv[1], "r");

    int n; fscanf(f, "%d", &n);

    char* s = new char[n+5];

    fscanf(f, "%s", s);

    bool mask1[] = {1, 0, 1, 1};
    int d = 6;

    clock_t start = clock();

    int *SA = new int[n + 5];
    divsufsort((unsigned char *)s, SA, n);
    printf("Vrijeme izgradnje SA: %lf\n", (double)(clock() - start) / CLOCKS_PER_SEC * 1000);
    
    start = clock();
    int *SA_masked = IPM::get_masked_SA(s, n, mask1, d);
    printf("Vrijeme izgradnje SSA: %lf\n", (double)(clock() - start) / CLOCKS_PER_SEC * 1000);
   
    char* p = new char[n+5];

    ///////////////////////////////////////

    int m = 100;
    memcpy(p, s + n / 4, m);
    p[m] = '\0';

    int idx;
    start = clock();
    IPM::all_occurrences(s, n, mask1, d, p, m, SA_masked, &idx);
    printf("Vrijeme pretrage SSA: %lf\n", (double)(clock() - start) / CLOCKS_PER_SEC * 1000);

    start = clock();
    sa_search((unsigned char*) s, n, (unsigned char*) p, m, SA, n, &idx);
    printf("Vrijeme pretrage SA: %lf\n", (double)(clock() - start) / CLOCKS_PER_SEC * 1000);

    delete[](s);
    delete[](p);
    delete[](SA_masked);
    
    return 0;
}

