#include <cstdio>
#include <cstring>
#include <algorithm>

using namespace std;

#define CHAR_NUMBER 27

void determine_sign_type(char *S, int n, bool *sign_type) {
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

void initialize_buckets(char *S, int n, int *B, int *B_start) {
	for(int i = 0; i < CHAR_NUMBER; ++i) {
		B[i] = 0;
	}
	for(int i = 0; i < n - 1; ++i) {
		++B[S[i] - 'A' + 1];
	}
	B_start[0] = 0;
	
	for(int i = 1; i < CHAR_NUMBER; ++i) {
		B[i] += B[i-1];
		B_start[i] = B[i] != B[i-1] ? B[i-1] + 1 : 0;
	}
}

void induced_sort(int *SA, char *S, int *B, int *B_start, int n, int *LMS, int n_LMS, bool *sign_type) {
	int B_end[100];
	for(int i = 0; i < n; ++i) {
		SA[i] = -1;
		B_end[i] = B[i];
	}

	for(int i = 0; i < n_LMS; ++i) {
		SA[B[max(0, S[LMS[i]] - 'A' + 1)]--] = LMS[i];
	}
	
	for(int i = 0; i < n; ++i) {
		if(SA[i] <= 0) continue;
		if(!sign_type[SA[i] - 1]) {
			SA[B_start[max(0, S[SA[i] - 1] - 'A' + 1)]++] = SA[i] - 1;
		}
	}

	for(int i = n - 1; i >= 0; --i) {
		if(SA[i] <= 0) continue;
		if(sign_type[SA[i] - 1]) {
			SA[B_end[max(0, S[SA[i] - 1] - 'A' + 1)]--] = SA[i] - 1;
		}
	}
}

int main(void) {

	char S[10000];
	int n;
	scanf("%d %s", &n, S);
	bool s[10000];
	int LMS[100];
	int B[100];
	int B_start[100];
	int SA[100];
	int n_LMS = 0;
	determine_sign_type(S, n, s);
	determine_LMS(s, n, LMS, &n_LMS);
	initialize_buckets(S, n, B, B_start);
	induced_sort(SA, S, B, B_start, n, LMS, n_LMS, s);
	
	for(int i = 0; i < n; ++i) printf("%d ", SA[i]);
	return 0;
}