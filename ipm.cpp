#include <cstdio>
#include <cstring>
#include <set>
#include <string>
#include <map>

#include "esa.cpp"

void solve(char* _str, int n, bool* mask, int m) {
    
    int n_padding = 2 * m - 1;
    if (n % m > 0) n_padding += m - (n % m);
    int n_padded = n + n_padding;
    
    char *str = new char[n_padded + 1];
    memcpy(str, _str, n);
    
    for(int i = 0; i < n_padding; ++i) {
        str[n + i] = (char)(n_padding - i);
    }
    
    str[n_padded] = '\0';
    
    printf("%s\n", str);
    
    std::set<std::string> masked_substrings;
    
    for(int i = 0; i < n_padded - m + 1; ++i) {
        char *_substr = new char[m];
        strncpy(_substr, str + i, m);
        for(int j = 0; j < m; ++j) {
            if(!mask[j]) _substr[j] = '*';
        }
        std::string substr(_substr);
        masked_substrings.insert(substr);
    }

    std::map<std::string, char> substring_ranks;
    
    char k = 0;
    for(std::set<std::string> :: iterator it = masked_substrings.begin(); it != masked_substrings.end(); ++it) {
        substring_ranks[*it] = k++;
        printf("%d %s\n", k - 1, (*it).c_str());
    }
    
    char* dislex = new char[n_padded - m + 2];
    k = 0;
    
    for(int i = 0; i < m; ++i) {
        for(int j = i; j < n_padded - m + 1; j += m) {
            char *_substr = new char[m];
            strncpy(_substr, str + j, m);
            for(int l = 0; l < m; ++l) {
                if(!mask[l]) _substr[l] = '*';
            }
            std::string substr(_substr);
            dislex[k++] = substring_ranks[substr] + 'a';
        }
    }
    
    dislex[n_padded - m + 1] = '\0';
    
    ESA* esa = new ESA(dislex, n_padded - m + 1);
    int* SA = esa->get_SA();
    for(int i = 0; i < n_padded - m + 1; ++i) {
        printf("%d ", SA[i]);
    }

    printf("\n");
    int t = (n_padded + 1) / m - 2;

    for(int i = 0; i < n_padded - m + 1; ++i) {
        printf("%d ", (SA[i] % (t + 1)) * m + (SA[i] / (t + 1)));
    }

}


int main(void) {

    char s[100];
    
    scanf("%s", s);
    
    bool mask[] = {true, false, true};
    
    solve(s, strlen(s), mask, 3);

    return 0;
}