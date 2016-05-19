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

int* IPM::get_masked_SA(char* _str, int n, bool* mask, int m, char *pattern, int n_pattern) {
    
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
    
    for(int i = 0; i < n_padded - m + 1; ++i) {
        char *_substr = new char[m + 1];
        strncpy(_substr, str + i, m);
        _substr[m] = '\0';
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
    
    char* dislex = new char[n_padded - m + 2];
    k = 0;
    
    for(int i = 0; i < m; ++i) {
        for(int j = i; j < n_padded - m + 1; j += m) {
            char *_substr = new char[m + 1];
            _substr[m] = '\0';
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
    delete(dislex);
    
    int* SA = esa->get_SA();

    int t = (n_padded + 1) / m - 2;
    
    int* SA_masked = new int[n];    
    str[n + 1] = '\0';
    for(int i = n_padding - m + 1; i < n_padded - m + 1; ++i) {
        SA_masked[i - n_padding + m - 1] = (SA[i] % (t + 1)) * m + (SA[i] / (t + 1));
    }
    
    return SA_masked;
}

int IPM::all_occurrences(char* str, int n, bool* mask, int m, char *pattern, int n_pattern, int* SA_masked, std::vector<int>& v) {

    for(int i = 0; i < n_pattern; ++i) {
        if(!mask[i % m]) pattern[i] = '*';
    }

    int left, right;
    int l = 0, r = n;
    
    while (l <= r) {
        int mid = l + (r - l)/2;
        int res = cmp(str + SA_masked[mid], mask, m, pattern, n_pattern);
        int res_prev = mid > 0 ? cmp(str + SA_masked[mid - 1], mask, m, pattern, n_pattern) : -1;
        if (res == 0 && res_prev < 0) {
            left = mid;
            break;
        }
        if (res < 0) {
            l = mid + 1;
        } else {
            r = mid - 1;
        }
    }
    
    l = 0, r = n;
    while (l <= r) {
        int mid = l + (r - l)/2;
        int res = cmp(str + SA_masked[mid], mask, m, pattern, n_pattern);
        int res_next = mid < n - 1 ? cmp(str + SA_masked[mid + 1], mask, m, pattern, n_pattern) : 1;
        if (res == 0 && res_next > 0) {
            right = mid;
            break;
        }
        if (res > 0) {
            r = mid - 1;
        } else {
            l = mid + 1;
        }
    }
    
    if(right < left) return -1;

    for(int i = right; i >= left; --i) {
        v.push_back(SA_masked[i]);
    }
    
    return SA_masked[right];
}
