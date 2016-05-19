#include <cstdio>
#include <cstring>
#include <set>
#include <string>
#include <vector>
#include <map>

#include "ipm.hpp"
#include "esa.hpp"

int IPM::cmp(char *str, bool *mask, int m, char *pattern, int n_pattern) {    
    for(int i = 0; i < n_pattern; ++i) {
        if(str[i] == '\0') return -1;
        if(!mask[i % m]) continue;
        if(str[i] < pattern[i]) return -1;
        if(str[i] > pattern[i]) return 1;
    }
    return 0;
}

int* IPM::get_masked_SA(char* _str, int n, bool* mask, int m) {

    int n_padding = 2 * m - 1;
    if (n % m > 0) n_padding += m - (n % m);
    int n_padded = n + n_padding;
    
    char *str = new char[n_padded + 1];
    memcpy(str, _str, n);
    
    for(int i = 0; i < n_padding; ++i) {
        str[n + i] = (char)(n_padding - i);
    }
    
    str[n_padded] = '\0';

    std::set<std::string> masked_substrings;

    char *_substr = new char[m + 1];
    _substr[m] = '\0';

    for(int i = 0; i < n_padded - m + 1; ++i) {
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
    }

    char* dislex = new char[n_padded - m + 1];
    int d = 0;
    
    for(int i = 0; i < m; ++i) {
        for(int j = i; j < n_padded - m + 1; j += m) {
            //printf("%d\n", j);
            strncpy(_substr, str + j, m);
            for(int l = 0; l < m; ++l) {
                if(!mask[l]) _substr[l] = '*';
            }
            std::string substr(_substr);
            printf("%s\n", _substr);
            dislex[d++] = substring_ranks[substr] + 'A';
        }
    }
    delete(_substr);
    printf("yo1\n");
    printf("%s\n", dislex);
    ESA* esa = new ESA(dislex, n_padded - m + 1, false);
    printf("yo2\n");
    delete(dislex);
    
    int* SA = esa->get_SA();

    int t = (n_padded + 1) / m - 2;
    
    int* SA_masked = new int[n];    
    str[n] = '\0';
    for(int i = n_padding - m + 2; i < n_padded - m + 2; ++i) {
        int x = (SA[i] % (t + 1)) * m + (SA[i] / (t + 1));
        SA_masked[i - n_padding + m - 2] = (SA[i] % (t + 1)) * m + (SA[i] / (t + 1));
        printf("%d %s\n", (SA[i] % (t + 1)) * m + (SA[i] / (t + 1)), str + (SA[i] % (t + 1)) * m + (SA[i] / (t + 1)));
    }
    
    delete(esa);
    
    return SA_masked;
}

int IPM::first_occurrence(char* str, int n, bool* mask, int m, char *pattern, int n_pattern, int* SA_masked, std::vector<int>& v) {
    int l = 0, r = n;
    while (l <= r) {
        int mid = l + (r - l)/2;
        int res = cmp(str + SA_masked[mid], mask, m, pattern, n_pattern);
        int res_next = mid < n - 1 ? cmp(str + SA_masked[mid + 1], mask, m, pattern, n_pattern) : 1;
        if (res == 0 && res_next > 0) {
            return SA_masked[mid];
            break;
        }
        if (res > 0) {
            r = mid - 1;
        } else {
            l = mid + 1;
        }
    }
    return -1;
}

int IPM::all_occurrences(char* str, int n, bool* mask, int m, char* pattern, int n_pattern, int* SA_masked, std::vector<int>& v) {

    int left, right;
    int l = 0, r = n - 1;
    bool found = false;
    
    while (l <= r) {
        int mid = l + (r - l)/2;
        int res = cmp(str + SA_masked[mid], mask, m, pattern, n_pattern);
        int res_next = mid < n - 1 ? cmp(str + SA_masked[mid + 1], mask, m, pattern, n_pattern) : 1;
        if (res == 0 && res_next > 0) {
            right = mid;
            found = true;
            break;
        }
        if (res > 0) {
            r = mid - 1;
        } else {
            l = mid + 1;
        }
    }
    
    if(!found) return -1;
    
    l = 0, r = n - 1;
    found = false;
    
    while (l <= r) {
        int mid = l + (r - l)/2;
        int res = cmp(str + SA_masked[mid], mask, m, pattern, n_pattern);
        int res_prev = mid > 0 ? cmp(str + SA_masked[mid - 1], mask, m, pattern, n_pattern) : -1;
        if (res == 0 && res_prev < 0) {
            left = mid;
            found = true;
            break;
        }
        if (res < 0) {
            l = mid + 1;
        } else {
            r = mid - 1;
        }
    }
    
    if(!found) return -1;

    for(int i = right; i >= left; --i) {
        v.push_back(SA_masked[i]);
    }
    
    return right - left + 1;
}
