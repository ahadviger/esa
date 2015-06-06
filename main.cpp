#include <cstdio>
#include <cstring>

#include "lcpinterval.hpp"
#include "childtable.hpp"
#include "esa2.cpp"

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
    
    ESA2* esa = new ESA2(S, n);

	int m;
    scanf("%d", &m);
    char* P = new char[m];
    scanf("%s", P);
    
    esa->search(P, strlen(P));
    
    return 0;
}