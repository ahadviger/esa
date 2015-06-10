#include <cstdio>
#include <cstring>
#include <vector>

#include "lcpinterval.hpp"
#include "childtable.hpp"
#include "esa.cpp"

int main(int argc, char** argv) {
    
    int n;
    char *S;
    
    // ucitavanje teksta iz datoteke ili
    // sa standardnog ulaza
    if (argc > 1) {
        FILE *f = fopen(argv[1], "r");
        fscanf(f, "%d", &n);
        S = new char[n+5];
        fscanf(f, "%s", S);
    } else {
        scanf("%d", &n);
        S = new char[n+5];
        scanf("%s", S);
        n = strlen(S);
    }
    
    ESA* esa = new ESA(S, n);
    
    // ucitavanje uzorka sa standardnog ulaza
    int m;
    scanf("%d", &m);
    char* P = new char[m];
    scanf("%s", P);
    
    std::vector<int> v;
    printf("First occurence of P in S: %d\n", esa->all_occurences(P, strlen(P), v));
    printf("Occurrences of P in S: ");
    for(std::vector<int> :: iterator it = v.begin(); it != v.end(); ++it) {
        printf("%d ", *it);
    }
    
    printf("\n");
    
    std::vector<int> v1, v2;
    printf("Longest suffix-prefix overlap: ");
    printf("%d\n", esa->all_overlaps(P, strlen(P), v1, v2));
    printf("Suffix-prefix overlap lengths: ");
    for(std::vector<int> :: iterator it = v1.begin(); it != v1.end(); ++it) {
        printf("%d ", *it);
    }
    for(std::vector<int> :: iterator it = v2.begin(); it != v2.end(); ++it) {
        printf("%d ", *it);
    }
    
    printf("\n");
    
    return 0;
}