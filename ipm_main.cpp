#include <cstdio>
#include <cstring>
#include <vector>
#include <malloc.h>

#include "esa.hpp"
#include "ipm.hpp"

int main(int argc, char** argv) {

/*
    char s[100], p[100];
    
    scanf("%s", s);
    scanf("%s", p);
    
    bool mask[] = {true, false, false, true};
    
    int *SA_masked = IPM::get_masked_SA(s, strlen(s), mask, 4);
    
    std::vector<int> v;
    
    printf("%d\n", IPM::all_occurrences(s, strlen(s), mask, 4, p, strlen(p), SA_masked, v));
    */
    
    FILE *f = fopen(argv[1], "r");

    int n; fscanf(f, "%d", &n);
    char* s = new char[n+5];
    fscanf(f, "%s", s);

    bool mask[] = {true, false, true};

    int *SA_masked = IPM::get_masked_SA(s, n, mask, 3);


    delete[](s);
    delete[](SA_masked);

    return 0;

    char p[100];
    scanf("%s", p);
    
    std::vector<int> v;
    
    printf("%d\n", IPM::all_occurrences(s, n, mask, 3, p, strlen(p), SA_masked, v));
    
    
    
    return 0;
}
