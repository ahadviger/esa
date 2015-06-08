#include <algorithm>
#include <stack>
#include <cstring>
#include <vector>

#include "lcpinterval.hpp"
#include "childtable.hpp"
#define CHAR_NUMBER 100

// Klasa koja izgradjuje poboljsano sufiksno polje, odnosno: 
// sufiksno polje, inverzno sufiksno polje, polje najduljih 
// zajednickih prefiksa i tablicu djece.

class ESA {
 public:
     // Konstruktor. Prima znakovni niz i njegovu duljinu te za taj niz
     // izgradjuje poboljsano sufiksno polje.
     // Na kraj niza dodaje se znak '|' veci od svih drugih znakova
     // abecede kako nijedan sufiks ne bi bio prefiks drugog sufiksa.
     ESA(char* _str, int _n) {
         n = _n;
         str = new char[n+5];
         memcpy(str, _str, n);

         str[n++] = '|';
         str[n] = '\0';

         S = new int[n];
         SA = new int[n];
         ISA = new int[n];
         LCP = new int[n];
         childtab = new ChildTable[n+1];

         for (int i = 0; i < n; ++i) {
             S[i] = str[i];
         }

         normalize_string();
         S[n++] = 0;

         int r = std::max(CHAR_NUMBER, n) + 5;
         int* B = new int[r];
         int* B_start = new int[r];
         int* B_end = new int[r];

         solve_SA(S, SA, n, B, B_start, B_end);
         determine_ISA();
         determine_LCP();
         SA++; LCP++; n--;

         for (int i = 0; i < n + 1; ++i) {
             childtab[i].up = childtab[i].down = childtab[i].nextlIndex = -1;
         }
         construct_child_table();

         delete(B); delete(B_start); delete(B_end);
     }
    
     // Vraca sufiksno polje.
     int* get_SA() {
         return SA;
     }

     // Vraca inverzno sufiksno polje.
     int* get_ISA() {
         return ISA;
     }

     // Vraca polje najduljih zajednickih prefiksa.
     int* get_LCP() {
         return LCP;
     }

     // Vraca tablicu djece.
     ChildTable* get_child_table() {
         return childtab;
     }

     // Trazi sva pojavljivanja znakovnog niza p duljine m unutar
     // niza nad kojim je izgradjeno polje. Prima i vektor u koji
     // se spremaju indeksi svih pojavljivanja.
     // Vraca indeks prvog pojavljivanja unutar niza.
     int search(char *p, int m, std::vector<int>& v) {
         int c = 0, min = 0, i = 0, j = 0, lcp = 0;
         LcpInterval *interval = getInterval(0, n, p[c]);
         bool query_found = interval != NULL;

         while (interval != NULL && c < m && query_found) {
             i = interval->Lb;
             j = interval->Rb;
             if (i != j) {
                 lcp = interval->lcp;
                 min = std::min(lcp, m);
                 interval = (min != m) ? getInterval(i, j, p[min]) : NULL;
                 query_found = true;
                 for (int k = c; k < min; ++k) {
                     if (str[SA[i] + k] != p[k]) {
                         query_found = false;
                         break;
                     }
                 }
                 c = min;
             } else {
                 query_found = true;
                 for (int k = c; k < m; ++k) {
                     if (str[SA[i] + k] != p[k]) {
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
             for (int k = i; k <= j; ++k) {
                v.push_back(SA[k]);
             }
             return SA[i];
         }
         return -1;
     }
     
     // Trazi sva sufiks-prefiks preklapanja, odnosno sve sufikse
     // niza nad kojim je izgradjeno polje koji su ujedno prefiksni
     // zadanog niza p duljine m. Prima i vektor u koji se
     // spremaju duljine svih preklapanja.
     // Vraca duljinu maksimalnog sufiks-prefiks preklapanja.
     int overlap(char *p, int m, std::vector<int>& v) {
         int c = 0, min = 0, i = 0, j = 0, lcp = 0;
         LcpInterval *interval = getInterval(0, n, p[c]);
         bool query_found = interval != NULL;
         int max = -1;
         while (interval != NULL && c < m && query_found) {
             i = interval->Lb;
             j = interval->Rb;
             if (i != j) {
                 lcp = interval->lcp;
                 min = std::min(lcp, m);
                 interval = (min != m) ? getInterval(i, j, p[min]) : NULL;
                 query_found = true;
                 for (int k = c; k < min; ++k) {
                     if (str[SA[j] + k] != p[k]) {
                         query_found = false;
                         break;
                     }
                  }
                 if (query_found && n - SA[j] == min + 1) {
                    v.push_back(n - 1 - SA[j]);
                    max = SA[j];
                 }
                 c = min;
             } else {
                 query_found = true;
                 for (int k = c; SA[i] + k < n - 1; ++k) {
                     if (k > m || str[SA[i] + k] != p[k]) {
                         query_found = false;
                         break;
                     }
                 }
                 if (query_found) {
                    max = SA[i];
                    v.push_back(n - 1 - SA[i]);
                 }
                 interval = NULL;
             }
         }
         if (max != -1) {
            return n - 1 - max;
         } else {
            return -1;
         }
     }

     // Vraca indeks prvog pojavljivanja niza p unutar
     // niza nad kojim je izgradjeno polje, no za pretragu
     // koristi samo sufiksno polje.
     int search_SA(char *p, int m) {
        int l = 0, r = n - 1;
        while (l <= r) {
            int mid = l + (r - l)/2;
            int res = strncmp(p, str + SA[mid], m);
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

     // Ispisuje sortirane sufikse.
     void print_sorted() {
         for (int i = 0; i < n; ++i) {
             printf("%d. %s\n", i + 1, str + SA[i]);
         }
     }

 private:
     char *str;
     int n, *S, *SA;
     int *LCP, *ISA;
     ChildTable *childtab;

     // Odredjuje je li pojedini znak niza L-tip ili S-tip znaka.
     void determine_char_type(int *S, int n, bool *char_type) {
         // 0 = L-type, 1 = S-type
         bool suffix_type = true;

         for (int i = n - 1; i >= 0; --i) {
             if (i == n - 1) {
                 char_type[i] = true;
                 continue;
             }
             if (S[i] == S[i+1]) {
                 char_type[i] = suffix_type;
             } else {
                 char_type[i] = S[i] < S[i+1];
                 suffix_type = char_type[i];
             }
         }
     }

     // U niz sprema indekse svih LMS-znakova unutar niza.
     void determine_LMS(bool *char_type, int n, int *LMS, int *n_LMS) {
         for (int i = 1; i < n; ++i) {
             if (char_type[i] && !char_type[i-1]) LMS[(*n_LMS)++] = i;
         }
     }

     // Provjerava je li znak na i-tom indeksu LMS-znak.
     bool is_LMS(bool *char_type, int i) {
         if (i == 0) return false;
         return (char_type[i] && !char_type[i-1]);
     }

     // Postavlja pokazivace na krajeve (B) i pocetke potpolja (B_start).
     void initialize_buckets(int *S, int n, int *B, int *B_start) {
         int r = std::max(CHAR_NUMBER, n);
         for (int i = 0; i < r; ++i) {
             B[i] = 0;
         }

         for (int i = 0; i < n - 1; ++i) {
             ++B[S[i]];
         }
         B_start[0] = 0;

         for (int i = 1; i < r; ++i) {
             B[i] += B[i-1];
             B_start[i] = B[i] != B[i-1] ? B[i-1] + 1 : 0;
         }
     }

     // Inducirano sortiranje LMS-podnizove.
     void induced_sort(int *SA, int *S, int *B, int *B_start, int *B_end, int n, int *LMS, int n_LMS, bool *char_type) {
         int r = std::max(CHAR_NUMBER, n);

         for (int i = 0; i < n; ++i) {
             SA[i] = -1;
         }

         for (int i = 0; i < r; ++i) {
             B_end[i] = B[i];
         }

         for (int i = n - 1; i >= 0; --i) {
             if (is_LMS(char_type, i)) SA[B[S[i]]--] = i;
         }

         for (int i = 0; i < n; ++i) {
             if (SA[i] <= 0) continue;
             if (!char_type[SA[i] - 1]) {
                 SA[B_start[S[SA[i] - 1]]++] = SA[i] - 1;
             }
         }

         for (int i = n - 1; i >= 0; --i) {
             if (SA[i] <= 0) continue;
             if (char_type[SA[i] - 1]) {
                 SA[B_end[S[SA[i] - 1]]--] = SA[i] - 1;
             }
         }
     }

     // Oduzimanje najmanje vrijednosti od svih clanova niza (najmanja postaje 1).
     void normalize_string() {
         int min = S[0];
         for (int i = 1; i < n; ++i) {
             if (S[i] < min) min = S[i];
         }
         for (int i = 0; i < n; ++i) {
             S[i] = S[i] - min + 1;
         }
     }

     // Provjerava jesu li dva LMS-podniza jednaka.
     bool compare_LMS(int *S, bool *char_type, int n, int n1, int n2) {
         if (n1 == n - 1 || n2 == n - 1) {
             return n1 == n2;
         }
         int e1 = 0;
         int e2 = 0;
         for (e1 = n1 + 1; e1 < n; ++e1) {
             if (is_LMS(char_type, e1)) break;
         }
         for (e2 = n2 + 1; e2 < n; ++e2) {
             if (is_LMS(char_type, e2)) break;
         }
         if (e1 - n1 != e2 - n2) return false;
         for (int i = 0; i < e1 - n1 + 1; ++i) {
             if (S[n1+i] != S[n2+i] || char_type[n1+i] != char_type[n2+i]) {
                 return false;
             }
         }
         return true;
     }

     // Imenovanje LMS-podnizova.
     void name_substrings(int *S1, int *SA, int *S, int *LMS, int n, int n_LMS, bool *char_type, bool *unique) {
         int name = 0;
         int* named = new int[n];
         int last = -1;
         *unique = true;

         for (int i = 0; i < n; ++i) {
             if (!is_LMS(char_type, SA[i])) continue;
             if (i == 0) {
                 named[SA[i]] = name;
                 last = SA[i];
                 continue;
             }
             if (!compare_LMS(S, char_type, n, SA[i], last)) {
                 ++name;
             } else {
                 *unique = false;
             }
             named[SA[i]] = name;
             last = SA[i];
         }

         for (int i = 0; i < n_LMS; ++i) {
             S1[i] = named[LMS[i]];
         }

         delete(named);
     }

     // Odredjivanje SA iz SA1.
     void determine_SA(int *SA, int *S, int *SA1, int *LMS, int *B, int *B_start, int *B_end, bool *char_type, int n, int n_LMS) {
         int r = std::max(CHAR_NUMBER, n);

         for (int i = 0; i < n; ++i) {
             SA[i] = -1;
         }

         for (int i = 0; i < r; ++i) {
             B_end[i] = B[i];
         }

         for (int i = n_LMS - 1; i >= 0; --i) {
             SA[B[S[LMS[SA1[i]]]]--] = LMS[SA1[i]];
         }

         for (int i = 0; i < n; ++i) {
             if (SA[i] <= 0) continue;
             if (!char_type[SA[i] - 1]) {
                 SA[B_start[S[SA[i] - 1]]++] = SA[i] - 1;
             }
         }
         for (int i = n - 1; i >= 0; --i) {
             if (SA[i] <= 0) continue;
             if (char_type[SA[i] - 1]) {
                 SA[B_end[S[SA[i] - 1]]--] = SA[i] - 1;
             }
         }
     }

     // Odredjivanje sufiksnog polja.
     void solve_SA(int *S, int *SA, int n, int *B, int *B_start, int *B_end) {
         bool* s = new bool[n];
         int* LMS = new int[n/2+1];
         int* S1 = new int[n/2+1];
         int* SA1 = new int[n/2+1];
         int n_LMS = 0;
         bool unique;

         determine_char_type(S, n, s);
         determine_LMS(s, n, LMS, &n_LMS);
         initialize_buckets(S, n, B, B_start);
         induced_sort(SA, S, B, B_start, B_end, n, LMS, n_LMS, s);
         name_substrings(S1, SA, S, LMS, n, n_LMS, s, &unique);

         if (!unique) {
             // rekurzivni poziv ako nisu sva imena jedinstvena
             solve_SA(S1, SA1, n_LMS, B, B_start, B_end);
         } else {
             // izravno odredjivanje SA1 iz S1
             for (int i = 0; i < n_LMS; ++i) {
                 SA1[S1[i]] = i;
             }
         }
         initialize_buckets(S, n, B, B_start);
         determine_SA(SA, S, SA1, LMS, B, B_start, B_end, s, n, n_LMS);

         delete(s); delete(LMS); delete(S1); delete(SA1);
     }

     // Odredjivanje inverznog sufiksnog polja.
     void determine_ISA() {
         for (int i = 0; i < n; ++i) {
             ISA[SA[i]] = i;
         }
     }

     // Odredjivanje polja najduljih zajednickih prefiksa.
     void determine_LCP() {
         int l = 0;
         for (int i = 0; i < n; ++i) {
             int k = ISA[i];
             int j = SA[k-1];
             while (i + l < n && j + l < n && S[i+l] == S[j+l]) {
                 ++l;
             }
             LCP[k] = l;
             if (l > 0) --l;
         }
     }

     // Odredjivanje up, down i nextlIndex vrijednosti tablice djece.
     void construct_child_table() {
         int last_index = -1;
         std::stack<int> stack;
         stack.push(0);
         for (int i = 1; i < n; ++i) {
             while (LCP[i] < LCP[stack.top()]) {
                 last_index = stack.top();
                 stack.pop();
                 if (LCP[i] <= LCP[stack.top()] && LCP[stack.top()] != LCP[last_index]) {
                     childtab[stack.top()].down = last_index;
                 }
             }
             if (LCP[i] >= LCP[stack.top()]) {
                 if (last_index != -1) {
                     childtab[i].up = last_index;
                     last_index = -1;
                 }
                 stack.push(i);
             }
         }

         while (!stack.empty()) stack.pop();

         stack.push(0);
         for (int i = 1; i < n; ++i) {
             while (LCP[i] < LCP[stack.top()]) {
                 stack.pop();
             }
             if (LCP[i] == LCP[stack.top()]) {
                 last_index = stack.top();
                 stack.pop();
                 childtab[last_index].nextlIndex = i;
             }
             stack.push(i);
         }
     }

     // Vraca LCP-vrijednost intervala [i..j].
     int getlcp(int i, int j) {
         if (i < childtab[j+1].up && j >= childtab[j+1].up) {
             return LCP[childtab[j+1].up];
         } else {
             return LCP[childtab[i].down];
         }
     }

     // Trazi dijete LCP-intervala [i..j] ciji sufiksi imaju
     // znak a na l-tom mjestu gdje je l LCP-vrijednost tog intervala.
     LcpInterval* getInterval(int i, int j, char a) {
         int _n = 0, m = 0, lcp = 0;
         if (i < 0 || i >= j) return NULL;
         if (i == 0 && j == n) {
             _n = 0;
             while (childtab[_n].nextlIndex != -1) {
                 m = childtab[_n].nextlIndex;
                 if (str[SA[_n]] == a) {
                     return new LcpInterval(getlcp(_n, m - 1), _n, m - 1);
                 }
                 _n = m;
             }
             if (str[SA[_n]] == a) {
                 return new LcpInterval(getlcp(_n, n - 1), _n, n - 1);
             }
         } else {
             lcp = getlcp(i, j);
             if (i < childtab[j+1].up && j >= childtab[j+1].up) {
                 _n = childtab[j+1].up;
             } else {
                 _n = childtab[i].down;
             }
             if (str[SA[i] + lcp] == a) {
                 return new LcpInterval(getlcp(i, _n - 1), i, _n - 1);
             }

             while (childtab[_n].nextlIndex != -1) {
                 m = childtab[_n].nextlIndex;
                 if (str[SA[_n] + lcp] == a) {
                     return new LcpInterval(getlcp(_n, m - 1), _n, m - 1);
                 }
                 _n = m;
             }
             if (str[SA[_n] + lcp] == a) {
                 return new LcpInterval(getlcp(_n, j), _n, j);
             }
         }
         return NULL;
     }
};
