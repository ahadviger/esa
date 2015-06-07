#include <cstdio>
#include <cstring>
#include <vector>

#include "lcpinterval.hpp"
#include "childtable.hpp"
#include "esa.cpp"

int main(int argc, char** argv) {
    
    int n;
    char *S;
    
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

    int m;
    scanf("%d", &m);
    char* P = new char[m];
    scanf("%s", P);
    
    std::vector<int> v;
    esa->search(P, strlen(P), v);
    printf("Occurrences of P in S: ");
    for(std::vector<int> :: iterator it = v.begin(); it != v.end(); ++it) {
        printf("%d ", *it);
    }
    
    printf("\n");
    
    std::vector<int> v2;
    printf("Longest suffix-prefix overlap: ");
    printf("%d\n", esa->search_suffix_prefix(P, strlen(P), v2));
    printf("Suffix-prefix overlap lengths: ");
    for(std::vector<int> :: iterator it = v2.begin(); it != v2.end(); ++it) {
        printf("%d ", *it);
    }
    
    return 0;
}