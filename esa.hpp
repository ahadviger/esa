#include "lcpinterval.hpp"
#include "childtable.hpp"
#define CHAR_NUMBER 100

class ESA {
	private:
		char *str;
		int n, *S, *SA, *lcptab;
		int *LCP, *ISA;
		ChildTable *childtab;
		
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
		
		void solve_SA(int *S, int *SA, int n, int *B, int *B_start) {
		
			bool *s = (bool*) malloc((n + 1) * sizeof(bool));
			int *LMS = (int*) malloc((n / 2 + 1) * sizeof(int));
			int *S1 = (int*) malloc((n / 2 + 1) * sizeof(int));
			int *SA1 = (int*) malloc((n / 2 + 1) * sizeof(int));
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
				solve_SA(S1, SA1, n_LMS, B, B_start);
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
			
			free(s); free(LMS); free(S1); free(SA1);
		}

		void determine_ISA() {
			for(int i = 0; i < n; ++i) {
				ISA[SA[i]] = i;
			}
		}
		
		void determine_LCP() {
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

		void construct_child_table() {
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

		void getChildIntervals(std::vector< std::pair<int, int> >& intervalList, int i, int j) {
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

		int getlcp(int i, int j) {
			if(i < childtab[j+1].up && j >= childtab[j+1].up) {
				return lcptab[childtab[j+1].up];
			} else {
				return lcptab[childtab[i].down];
			}
		}

		LcpInterval* getInterval(int i, int j, char a) {
			int N = n;
			int n = 0, m = 0, lcp = 0;
			if(i < 0 || i >= j) return NULL;
			if(i == 0 && j == N) {
				n = 0;
				while(childtab[n].nextlIndex != 0) {
					m = childtab[n].nextlIndex;
					if(str[SA[n]] == a) {
						return new LcpInterval(getlcp(n, m - 1), n, m - 1);
					}
					n = m;
				}
			} else {
				lcp = getlcp(i, j);
				if(childtab[j+1].up <= j) {
					n = childtab[j+1].up;
				} else {
					n = childtab[i].down;
				}
				if(str[SA[i] + lcp] == a) {
					return new LcpInterval(getlcp(i, n - 1), i, n - 1);
				}
				while(childtab[n].nextlIndex != 0) {
					m = childtab[n].nextlIndex;
					if(str[SA[n] + lcp] == a) {
						return new LcpInterval(getlcp(n, m - 1), n, m - 1);
					}
					n = m;
				}
				if(str[SA[n] + lcp] == a) {
					return new LcpInterval(getlcp(n, j), n, j);
				}
			}
			return NULL;
		}
		
	public:
		ESA() {}
		ESA(char *_str) {
			str = _str; n = strlen(str);
			S = (int*) malloc((n + 1) * sizeof(int));
			for(int i = 0; i < n; ++i) {
				S[i] = str[i];
			}
			SA = ISA = LCP = lcptab = NULL; childtab = NULL;
		}

		int* get_SA() {
			if(SA == NULL) {
				SA = (int*) malloc((n + 1) * sizeof(int));
				int r = std::max(CHAR_NUMBER, n) + 1;
				int *B = (int*) malloc(r * sizeof(int));
				int *B_start = (int*) malloc(r * sizeof(int));
				solve_SA(S, SA, n, B, B_start);
				free(B); free(B_start);
			}
			++n;
			return SA + 1;
		}

		int* get_ISA() {
			get_SA();
			if(ISA == NULL) {
				ISA = (int*) malloc((n + 1) * sizeof(int));
				determine_ISA();
			}
			return ISA + 1;
		}

		int* get_LCP() {
			get_SA(); get_ISA();
			if(LCP == NULL) {
				LCP = (int*) malloc((n + 1) * sizeof(int));
				determine_LCP();
			}
			return LCP + 1;
		}
		
		ChildTable* get_child_table() {
			get_LCP();
			if(childtab == NULL) {
				childtab = (ChildTable*) malloc((n + 1) * sizeof(ChildTable));
				for(int i = 0; i < n; ++i) {
					childtab[i].up = childtab[i].down = childtab[i].nextlIndex = -1;
				}
				construct_child_table();
			}
			return childtab;
		}
		
		int search (char *pattern, int len) {}
		
		void print_sorted() {
			get_SA();
			for(int i = 1; i < n; ++i) {
				 printf("%s\n", str + SA[i]);
			}
		}
};