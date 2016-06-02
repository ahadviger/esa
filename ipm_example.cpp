#include <cstdio>
#include <cstring>
#include <vector>
#include <time.h>

#include "esa.hpp"
#include "ipm.hpp"

int main(int argc, char** argv) {

    FILE *f = fopen(argv[1], "r");

    int n; fscanf(f, "%d", &n);

    char* s = new char[n+5];

    fscanf(f, "%s", s);

    bool mask[] = {1, 0, 1, 1};
    int d = 6;

    clock_t start = clock();

    int *SA_masked = IPM::get_masked_SA(s, n, mask, d);
    printf("Vrijeme izgradnje SSA: %lf\n", (double)(clock() - start) / CLOCKS_PER_SEC * 1000);
   
    ///////////////////////////////////////

    int m;
    scanf("%d", &m);
    char* p = new char[m+5];
    scanf("%s", p);

    int idx;
    start = clock();
    int x = IPM::all_occurrences(s, n, mask, d, p, m, SA_masked, &idx);
    printf("Prvo pojavljivanje: %d\nBroj pojavljivanja: %d\n", idx, x);
    printf("Vrijeme pretrage SSA: %lf\n", (double)(clock() - start) / CLOCKS_PER_SEC * 1000);

    delete[](s);
    delete[](p);
    delete[](SA_masked);
    
    return 0;
}

