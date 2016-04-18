#include <cstdio>
#include <cstring>
#include <set>
#include <string>
#include <map>

#include "esa.cpp"

int cmp(char *str, bool *mask, int m, char *pattern, int n_pattern) {    
    for(int i = 0; i < n_pattern; ++i) {
        if(str[i] == '\0') return 1;
        if(!mask[i % m]) continue;
        if(str[i] < pattern[i]) return 1;
        if(str[i] > pattern[i]) return -1;
    }
    return 0;
}

void solve(char* _str, int n, bool* mask, int m, char *pattern, int n_pattern) {
    
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
    
    int* SA_masked = new int[n_padded - m + 1];
    
    for(int i = 0; i < n_padded - m + 1; ++i) {
        printf("%s\n", str + (SA[i] % (t + 1)) * m + (SA[i] / (t + 1)));
        SA_masked[i] = (SA[i] % (t + 1)) * m + (SA[i] / (t + 1));
    }
    
    for(int i = 0; i < n_pattern; ++i) {
        if(!mask[i % m]) pattern[i] = '*';
    } 
    
    int l = 0, r = n_padded - m + 1;
    while (l <= r) {
        int mid = l + (r - l)/2;
        int res = cmp(str + SA_masked[mid], mask, m, pattern, n_pattern);
        printf("%s\n", str + SA[mid]);
        if (res == 0) {
            printf("%d\n", SA_masked[mid]);
            break;
        }
        if (res < 0) {
            r = mid - 1;
        } else {
            l = mid + 1;
        }
    }
}


int main(void) {

    char s[100], p[100];
    
    scanf("%s", s);
    scanf("%s", p);
    
    bool mask[] = {true, false, true};
    
    solve(s, strlen(s), mask, 3, p, strlen(p));

    return 0;
}