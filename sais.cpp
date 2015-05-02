#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;

#define CHAR_NUMBER 27

void determine_sign_type(int *S, int n, bool *sign_type) {
	// 0 = L-type, 1 = S-type
	bool suffix_type = true;

	for(int i = n - 1; i >= 0; --i) {
		if(i == n - 1) {
			sign_type[i] = true;
			continue;
		}
		if(S[i] == S[i+1]) {
			sign_type[i] = suffix_type;
		} else {
			sign_type[i] = S[i] < S[i+1];
			suffix_type = sign_type[i];
		}
	}
}

void determine_LMS(bool *sign_type, int n, int *LMS, int *n_LMS) {
	for(int i = 1; i < n; ++i) {
		if(sign_type[i] && !sign_type[i-1]) LMS[(*n_LMS)++] = i;
	}
}

bool is_LMS(bool *sign_type, int i) {
	if(i == 0) return false;
	return(sign_type[i] && !sign_type[i-1]);
}

void initialize_buckets(int *S, int n, int *B, int *B_start) {
	int r = max(CHAR_NUMBER, n);
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

void induced_sort(int *SA, int *S, int *B, int *B_start, int n, int *LMS, int n_LMS, bool *sign_type) {
	int B_end[100];
	for(int i = 0; i < n; ++i) {
		SA[i] = -1;
		B_end[i] = B[i];
	}

	for(int i = 0; i < n_LMS; ++i) {
		SA[B[S[LMS[i]]]--] = LMS[i];
	}
	
	for(int i = 0; i < n; ++i) {
		if(SA[i] <= 0) continue;
		if(!sign_type[SA[i] - 1]) {
			SA[B_start[S[SA[i] - 1]]++] = SA[i] - 1;
		}
	}

	for(int i = n - 1; i >= 0; --i) {
		if(SA[i] <= 0) continue;
		if(sign_type[SA[i] - 1]) {
			SA[B_end[S[SA[i] - 1]]--] = SA[i] - 1;
		}
	}
}

void normalize_string(char *str, int *S, int n) {
	int min = str[0];
	for(int i = 1; i < n; ++i) {
		if(str[i] < min) min = str[i];
	}
	for(int i = 0; i < n; ++i) {
		S[i] = str[i] - min + 1;
	}
}

bool compare_LMS(int *S, bool *sign_type, int n, int n1, int n2) {
	if(n1 == n - 1 || n2 == n - 1) {
		return n1 == n2;
	}
	int e1 = 0;
	int e2 = 0;
	for(e1 = n1 + 1;; ++e1) {
		if(is_LMS(sign_type, e1)) break;
	}
	for(e2 = n2 + 1;; ++e2) {
		if(is_LMS(sign_type, e2)) break;
	}
	if(e1 - n1 != e2 - n2) return false;
	for(int i = 0; i < e1 - n1 + 1; ++i) {
		if(S[n1+i] != S[n2+i] || sign_type[n1+i] != sign_type[n2+i]) return false;
	}
	return true;
}

void name_substrings(int *SA1, int *SA, int *S, int *LMS, int n, int n_LMS, bool *sign_type) {
	int name = 0;
	int *named = (int*) malloc(n * sizeof(int));
	int last = -1;
	for(int i = 0; i < n; ++i) {
		if(!is_LMS(sign_type, SA[i])) continue;
		if(i == 0) {
			named[SA[i]] = name;
			last = SA[i];
			continue;
		}
		if(!compare_LMS(S, sign_type, n, SA[i], last)) ++name;
		named[SA[i]] = name;
		last = SA[i];
	}
	for(int i = 0; i < n_LMS; ++i) {
		SA1[i] = named[LMS[i]];
	}
}

void determine_SA(int *SA, int *S, int *SA1, int *LMS, int *B, int *B_start, bool *sign_type, int n, int n_LMS) {
	int B_end[100];
	for(int i = 0; i < n; ++i) {
		SA[i] = -1;
		B_end[i] = B[i];
	}

	for(int i = n_LMS - 1; i >= 0; --i) {
		SA[B[S[LMS[SA1[i]]]]--] = LMS[SA1[i]];
	}
		
	for(int i = 0; i < n; ++i) {
		if(SA[i] <= 0) continue;
		if(!sign_type[SA[i] - 1]) {
			SA[B_start[S[SA[i] - 1]]++] = SA[i] - 1;
		}
	}

	for(int i = n - 1; i >= 0; --i) {
		if(SA[i] <= 0) continue;
		if(sign_type[SA[i] - 1]) {
			SA[B_end[S[SA[i] - 1]]--] = SA[i] - 1;
		}
	}
}

int main(void) {

	char SS[10000];
	int n;
	scanf("%d %s", &n, SS);
	int S[1000];
	bool s[10000];
	int LMS[100];
	int B[100];
	int B_start[100];
	int SA[100];
	int SA1[100];
	int n_LMS = 0;
	normalize_string(SS, S, n);
	S[n++] = 0;
	determine_sign_type(S, n, s);
	determine_LMS(s, n, LMS, &n_LMS);
	initialize_buckets(S, n, B, B_start);
	induced_sort(SA, S, B, B_start, n, LMS, n_LMS, s);
	name_substrings(SA1, SA, S, LMS, n, n_LMS, s);
	initialize_buckets(S, n, B, B_start);
	
	for(int i = 0; i < n_LMS; ++i) printf("%d ", LMS[i]);
	printf("\n");

	for(int i = 0; i < n; ++i) printf("%d ", SA[i]);
	printf("\n");
	for(int i = 0; i < n_LMS; ++i) printf("%d ", SA1[i]);
	printf("\n\n");
	determine_SA(SA, S, SA1, LMS, B, B_start, s, n, n_LMS);
	for(int i = 0; i < n; ++i) printf("%d ", SA[i]);
	return 0;
}