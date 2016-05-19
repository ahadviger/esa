#include <cstdio>
#include <cstring>
#include <vector>


#include "ipm.hpp"

int main(void) {

    char s[100], p[100];
    
    scanf("%s", s);
    scanf("%s", p);
    
    bool mask[] = {true, false, true};
    
    int *SA_masked = IPM::get_masked_SA(s, strlen(s), mask, 3, p, strlen(p));
    
    std::vector<int> v;
    
    printf("%d\n", IPM::all_occurrences(s, strlen(s), mask, 3, p, strlen(p), SA_masked, v));
    
    return 0;
}