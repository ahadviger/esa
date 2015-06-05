#include <cstdio>
#include <cstring>
#include <algorithm>
#include <stack>
#include <vector>
#include "lcpinterval.hpp"
#include "childtable.hpp"
#define CHAR_NUMBER 100


void determine_char_type(int *S, int n, bool *char_type) {
	// 0 = L-type, 1 = S-type
	bool suffix_type = true;

	for(int i = n - 1; i >= 0; --i) {
		if(i == n - 1) {
			char_type[i] = true;
			continue;
		}
		if(S[i] == S[i+1]) {
			char_type[i] = suffix_type;
		} else {
			char_type[i] = S[i] < S[i+1];
			suffix_type = char_type[i];
		}
	}
}

void determine_LMS(bool *char_type, int n, int *LMS, int *n_LMS) {
	for(int i = 1; i < n; ++i) {
		if(char_type[i] && !char_type[i-1]) LMS[(*n_LMS)++] = i;
	}
}

bool is_LMS(bool *char_type, int i) {
	if(i == 0) return false;
	return(char_type[i] && !char_type[i-1]);
}

void initialize_buckets(int *S, int n, int *B, int *B_start) {
	int r = std::max(CHAR_NUMBER, n);
	for(int i = 0; i < r; ++i) {
		B[i] = 0;
	}
	for(int i = 0; i < n - 1; ++i) {
		++B[S[i]];
	}
	B_start[0] = 0;
	
	for(int i = 1; i < r; ++i) {
		B[i] += B[i-1];
		B_start[i] = B[i] != B[i-1] ? B[i-1] + 1 : 0;
	}
}

void induced_sort(int *SA, int *S, int *B, int *B_start, int n, int *LMS, int n_LMS, bool *char_type) {
	int B_end[100];
	for(int i = 0; i < n; ++i) {
		SA[i] = -1;
	}
	int r = std::max(CHAR_NUMBER, n);
	for(int i = 0; i < r; ++i) {
		B_end[i] = B[i];
	}
	
	for(int i = n - 1; i >= 0; --i) {
		if(is_LMS(char_type, i)) SA[B[S[i]]--] = i;
	}
		
	for(int i = 0; i < n; ++i) {
		if(SA[i] <= 0) continue;
		if(!char_type[SA[i] - 1]) {
			SA[B_start[S[SA[i] - 1]]++] = SA[i] - 1;
		}
	}
	
	for(int i = n - 1; i >= 0; --i) {
		if(SA[i] <= 0) continue;
		if(char_type[SA[i] - 1]) {
			SA[B_end[S[SA[i] - 1]]--] = SA[i] - 1;
		}
	}
}

void normalize_string(int *S, int n) {
	int min = S[0];
	for(int i = 1; i < n; ++i) {
		if(S[i] < min) min = S[i];
	}
	for(int i = 0; i < n; ++i) {
		S[i] = S[i] - min + 1;
	}
}

bool compare_LMS(int *S, bool *char_type, int n, int n1, int n2) {
	if(n1 == n - 1 || n2 == n - 1) {
		return n1 == n2;
	}
	int e1 = 0;
	int e2 = 0;
	for(e1 = n1 + 1; e1 < n; ++e1) {
		if(is_LMS(char_type, e1)) break;
	}
	for(e2 = n2 + 1; e2 < n; ++e2) {
		if(is_LMS(char_type, e2)) break;
	}
	if(e1 - n1 != e2 - n2) return false;
	for(int i = 0; i < e1 - n1 + 1; ++i) {
		if(S[n1+i] != S[n2+i] || char_type[n1+i] != char_type[n2+i]) {
			return false;
		}
	}
	return true;
}

void name_substrings(int *S1, int *SA, int *S, int *LMS, int n, int n_LMS, bool *char_type, bool *unique) {
	int name = 0;
	int *named = (int*) malloc(n * sizeof(int));
	int last = -1;
	*unique = true;
	
	for(int i = 0; i < n; ++i) {
		if(!is_LMS(char_type, SA[i])) continue;
		if(i == 0) {
			named[SA[i]] = name;
			last = SA[i];
			continue;
		}
		if(!compare_LMS(S, char_type, n, SA[i], last)) {
			++name;
		} else {
			*unique = false;
		}
		named[SA[i]] = name;
		last = SA[i];
	}
	
	for(int i = 0; i < n_LMS; ++i) {
		S1[i] = named[LMS[i]];
	}
	
	free(named);
}

void determine_SA(int *SA, int *S, int *SA1, int *LMS, int *B, int *B_start, bool *char_type, int n, int n_LMS) {
	int B_end[100];
	for(int i = 0; i < n; ++i) {
		SA[i] = -1;
	}
	int r = std::max(CHAR_NUMBER, n);
	for(int i = 0; i < r; ++i) {
		B_end[i] = B[i];
	}

	for(int i = n_LMS - 1; i >= 0; --i) {
		SA[B[S[LMS[SA1[i]]]]--] = LMS[SA1[i]];
	}
	
	for(int i = 0; i < n; ++i) {
		if(SA[i] <= 0) continue;
		if(!char_type[SA[i] - 1]) {
			SA[B_start[S[SA[i] - 1]]++] = SA[i] - 1;
		}
	}

	for(int i = n - 1; i >= 0; --i) {
		if(SA[i] <= 0) continue;
		if(char_type[SA[i] - 1]) {
			SA[B_end[S[SA[i] - 1]]--] = SA[i] - 1;
		}
	}
}

void print_sorted(char *S, int *SA, int n) {
	for(int i = 0; i < n; ++i) {
		 printf("%s\n", S + SA[i]);
	}
}

void solve(int *S, int *SA, int n) {
	// printf("ulaz u solve\n");
	bool s[10000];
	int LMS[100];
	int B[100];
	int B_start[100];
	int S1[100];
	int SA1[100];
	int n_LMS = 0;
	bool unique;
	
	normalize_string(S, n);
	S[n++] = 0;
	determine_char_type(S, n, s);
	determine_LMS(s, n, LMS, &n_LMS);
	initialize_buckets(S, n, B, B_start);
	induced_sort(SA, S, B, B_start, n, LMS, n_LMS, s);
	name_substrings(S1, SA, S, LMS, n, n_LMS, s, &unique);
	
	if(!unique) {
		solve(S1, SA1, n_LMS);
		for(int i = 1; i < n_LMS + 1; ++i) {
			SA1[i-1] = SA1[i];
		}
	} else {
		for(int i = 0; i < n_LMS; ++i) {
			SA1[S1[i]] = i;
		}
	}
	initialize_buckets(S, n, B, B_start);
	determine_SA(SA, S, SA1, LMS, B, B_start, s, n, n_LMS);
}

void convert_to_int(char *SS, int *S, int n) {
	for(int i = 0; i < n; ++i) {
		S[i] = SS[i];
	}
}

void determine_ISA(int *ISA, int *SA, int n) {
	for(int i = 0; i < n; ++i) {
		ISA[SA[i]] = i;
	}
}

void determine_LCP(int *LCP, int *S, int *SA, int *ISA, int n) {
	if(ISA == NULL) {
		determine_ISA(ISA, SA, n);
	}
	
	int l = 0;
	for(int i = 0; i < n; ++i) {
		int k = ISA[i];
		int j = SA[k-1];
		while(i + l < n && j + l < n && S[i+l] == S[j+l]) {
			++l;
		}
		LCP[k] = l;
		if(l > 0) --l;
	}
}

void construct_child_table(ChildTable* childtab, int *LCP, int n) {
	int last_index = -1;
	std::stack<int> stack;
	stack.push(0);
	for(int i = 1; i < n; ++i) {
		while(LCP[i] < LCP[stack.top()]) {
			last_index = stack.top();
			stack.pop();
			if(LCP[i] <= LCP[stack.top()] && LCP[stack.top()] != LCP[last_index]) {
				childtab[stack.top()].down = last_index;
			}
		}
		if(LCP[i] >= LCP[stack.top()]) {
			if(last_index != -1) {
				childtab[i].up = last_index;
				last_index = -1;
			}
			stack.push(i);
		}
	}
	
	while(!stack.empty()) stack.pop();
	
	stack.push(0);
	for(int i = 1; i < n; ++i) {
		while(LCP[i] < LCP[stack.top()]) {
			stack.pop();
		}
		if(LCP[i] == LCP[stack.top()]) {
			last_index = stack.top();
			stack.pop();
			childtab[last_index].nextlIndex = i;
		}
		stack.push(i);
	}
}

void getChildIntervals(std::vector< std::pair<int, int> >& intervalList, ChildTable* childtab, int i, int j) {
	intervalList.clear();
	int i1, i2;
	if(i < childtab[j+1].up && childtab[j+1].up <= j) {
		i1 = childtab[j+1].up;
	} else {
		i1 = childtab[i].down;
	}
	intervalList.push_back(std::make_pair(i, i1 - 1));
	while(childtab[i1].nextlIndex != -1) {
		i2 = childtab[i1].nextlIndex;
		intervalList.push_back(std::make_pair(i1, i2 - 1));
		i1 = i2;
	}
	intervalList.push_back(std::make_pair(i1, j));
}

int getlcp(ChildTable *childtab, int *lcptab, int i, int j) {
	if(i < childtab[j+1].up && j >= childtab[j+1].up) {
		return lcptab[childtab[j+1].up];
	} else {
		return lcptab[childtab[i].down];
	}
}

LcpInterval* getInterval(int i, int j, char a, int *SA, ChildTable *childtab, int *lcptab, char *S, int N) {
    int n = 0, m = 0, lcp = 0;
    if (i < 0 || i >= j) return NULL;
    if (i == 0 && j == N) {
        n = 0;
        while (childtab[n].nextlIndex != -1) {
            m = childtab[n].nextlIndex;
            if (S[SA[n]] == a) {
                return new LcpInterval(getlcp(childtab, lcptab, n, m - 1), n, m - 1);
            }
            n = m;
        }
        if (S[SA[n]] == a) {
            return new LcpInterval(getlcp(childtab, lcptab, n, N - 1), n, N - 1);
        }
    } else {
        lcp = getlcp(childtab, lcptab, i, j);
        if (childtab[j+1].up <= j) {
            n = childtab[j+1].up;
        } else {
            n = childtab[i].down;
        }
        if (S[SA[i] + lcp] == a) {
            return new LcpInterval(getlcp(childtab, lcptab, i, n - 1), i, n - 1);
        }
        while (childtab[n].nextlIndex != -1) {
            m = childtab[n].nextlIndex;
            if (S[SA[n] + lcp] == a) {
                return new LcpInterval(getlcp(childtab, lcptab, n, m - 1), n, m - 1);
            }
            n = m;
        }
        if (S[SA[n] + lcp] == a) {
            return new LcpInterval(getlcp(childtab, lcptab, n, j), n, j);
        }
    }
    return NULL;
}

LcpInterval* getInterval_stari(int i, int j, char a, int *SA, ChildTable *childtab, int *lcptab, char *S, int N) {
	int n = 0, m = 0, lcp = 0;
	if(i < 0 || i >= j) return NULL;
	if(i == 0 && j == N) {
		n = 0;
		while(childtab[n].nextlIndex != 0) {
			m = childtab[n].nextlIndex;
			if(S[SA[n]] == a) {
				return new LcpInterval(getlcp(childtab, lcptab, n, m - 1), n, m - 1);
			}
			n = m;
		}
	} else {
		lcp = getlcp(childtab, lcptab, i, j);
		if(childtab[j+1].up <= j) {
			n = childtab[j+1].up;
		} else {
			n = childtab[i].down;
		}
		if(S[SA[i] + lcp] == a) {
			return new LcpInterval(getlcp(childtab, lcptab, i, n - 1), i, n - 1);
		}
		while(childtab[n].nextlIndex != 0) {
			m = childtab[n].nextlIndex;
			if(S[SA[n] + lcp] == a) {
				return new LcpInterval(getlcp(childtab, lcptab, n, m - 1), n, m - 1);
			}
			n = m;
		}
		if(S[SA[n] + lcp] == a) {
			return new LcpInterval(getlcp(childtab, lcptab, n, j), n, j);
		}
	}
	return NULL;
}

void search2(char *p, int len, int *SA, ChildTable *childtab, int *lcptab, char *S, int n) {
    int c = 0, min = 0, i = 0, j = 0, lcp = 0, m = len;
    LcpInterval *interval = getInterval(0, n, p[c], SA, childtab, lcptab, S, n);
    bool query_found = interval != NULL;
	while(interval != NULL && c < m && query_found) {
		i = interval->Lb;
		j = interval->Rb;
		if(i != j) {
			lcp = interval->lcp;
			min = std::min(lcp, m);
			interval = min != m ? getInterval(i, j, p[min], SA, childtab, lcptab, S, n) : NULL;
			query_found = true;
			for(int k = c; k < min; ++k) {
				if(S[SA[i] + k] != p[k]) {
					query_found = false;
					break;
				}
			}
			c = min;
		} else {
			query_found = true;
			for(int k = c; k < m; ++k) {
				if(S[SA[i] + k] != p[k]) {
					query_found = false;
					break;
				}
			}
			interval = NULL;
		}
	}
	if(query_found) {
		for(int ii = i; ii <= j; ++ii) printf("%d ", SA[ii]);
	}
}

void search3(char *p, int len, int *SA, ChildTable *childtab, int *lcptab, char *S, int n) {
    int c = 0, min = 0, i = 0, j = 0, lcp = 0, m = len;
    LcpInterval *interval = getInterval(0, n, p[c], SA, childtab, lcptab, S, n);
	i = interval->Lb;
	j = interval->Rb;
    bool query_found = true;
	while(interval != NULL && c < m && query_found) {
		if(i != j) {
			lcp = interval->lcp;
			min = std::min(lcp, m);
			query_found = true;
			for(int k = c; k < min; ++k) {
				if(S[SA[i] + k] != p[k]) {
					query_found = false;
					break;
				}
			}
			c = min;
			interval = getInterval(i, j, p[c], SA, childtab, lcptab, S, n);
			i = interval->Lb;
			j = interval->Rb;
		} else {
			query_found = true;
			for(int k = c; k < m; ++k) {
				if(S[SA[i] + k] != p[k]) {
					query_found = false;
					break;
				}
			}
		}
	}
	if(query_found) {
		for(int ii = i; ii <= j; ++ii) printf("%d ", SA[ii]);
	}
}

int search(char *S, char *pattern, int *SA, int n) {
	int m = strlen(pattern);
	int l = 0, r = n - 1;
	while (l <= r) {
		int mid = l + (r - l)/2;
		int res = strncmp(pattern, S + SA[mid], m);
        if (res == 0) {
			return SA[mid];
		}
		if (res < 0) {
			r = mid - 1;
		} else {
			l = mid + 1;
		}
	}
	return -1;
}

void search_novi(char *p, int len, int *SA, ChildTable *childtab, int *lcptab, char *S, int n) {
    int c = 0, min = 0, i = 0, j = 0, lcp = 0, m = len;
    LcpInterval *interval = getInterval(0, n, p[c], SA, childtab, lcptab, S, n);
    bool query_found = interval != NULL;
    while (interval != NULL && c < m && query_found) {
        i = interval->Lb;
        j = interval->Rb;
        if (i != j) {
            lcp = interval->lcp;
            min = std::min(lcp, m);
            interval = (min != m) ? getInterval(i, j, p[min], SA, childtab, lcptab, S, n) : NULL;
            query_found = true;
            for (int k = c; k < min; ++k) {
                if (S[SA[i] + k] != p[k]) {
                    query_found = false;
                    break;
                }
            }
            c = min;
        } else {
            query_found = true;
            for (int k = c; k < m; ++k) {
                if (S[SA[i] + k] != p[k]) {
                    query_found = false;
                    break;
                }
            }
			c = m;
            interval = NULL;
        }
    }
    if (c < m && interval == NULL) query_found = false;
    if (query_found) {
        for (int ii = i; ii <= j; ++ii) printf("%d ", SA[ii]);
    }
}

int search_suffix_prefix(char *p, int len, int *SA, ChildTable *childtab, int *lcptab, char *S, int n) {
    int c = 0, min = 0, i = 0, j = 0, lcp = 0, m = len;
    LcpInterval *interval = getInterval(0, n, p[c], SA, childtab, lcptab, S, n);
    bool query_found = interval != NULL;
    int max = -1;
    while (interval != NULL && c < m && query_found) {
        i = interval->Lb;
        j = interval->Rb;
        if (i != j) {
            lcp = interval->lcp;
            min = std::min(lcp, m);
            interval = (min != m) ? getInterval(i, j, p[min], SA, childtab, lcptab, S, n) : NULL;
            query_found = true;
            for (int k = c; k < min; ++k) {
                if (S[SA[j] + k] != p[k]) {
                    query_found = false;
                    break;
                }
            }
            if (query_found && n - SA[j] == min + 1) max = SA[j];
            c = min;
        } else {
            query_found = true;
            for (int k = c; SA[i] + k < n - 1; ++k) {
                if (k > m || S[SA[i] + k] != p[k]) {
                    query_found = false;
                    break;
                }
            }
            if (query_found) max = SA[i];
            interval = NULL;
        }
    }
    return max;
}

int main(void) {

	char SS[10000] = {1, 3, 1, 2, 0};
	char p[1000];
	int n;
	scanf("%s", SS);
	n = strlen(SS);
	SS[n++] = '|';
	int S[10000];
	convert_to_int(SS, S, n);
	normalize_string(S, n);
	int SA[100];
	int ISA[100];
	int LCP[100];
	solve(S, SA, n);
	determine_ISA(ISA, SA, n + 1);
	determine_LCP(LCP, S, SA, ISA, n + 1);
	for(int i = 1; i < n + 1; ++i) {
		printf("%d ", SA[i]);
		SA[i-1] = SA[i];
	}
	printf("\n");
	for(int i = 1; i < n + 1; ++i) {
		printf("%d ", LCP[i]);
		LCP[i-1] = LCP[i];
	}
	
	printf("\n");
	print_sorted(SS, SA, n);
	

	ChildTable childtab[100];
	for(int i = 0; i < n + 1; ++i) {
		childtab[i].up = childtab[i].down = childtab[i].nextlIndex = -1;
	}
	construct_child_table(childtab, LCP, n);
	for(int i = 0; i < n + 1; ++i) {
		printf("%3d %3d %3d\n", childtab[i].up, childtab[i].down, childtab[i].nextlIndex);
	}
	
	char PAT[100];
	scanf("%s", PAT);
	printf("%s\n", SS);
	printf("%d\n", search_suffix_prefix(PAT, strlen(PAT), SA, childtab, LCP, SS, n));
	
	return 0;
}