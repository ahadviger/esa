#pragma once

#include <vector>

#include "lcpinterval.hpp"
#include "childtable.hpp"

class IPM {
 public:  
     // Vraca maskirano sufiksno polje.
     static int* get_masked_SA(char* _str, int n, bool* mask, int m);

     // Trazi sva pojavljivanja znakovnog niza pattern pod maskom mask
     // unutar niza nad kojim je izgradjeno polje. Prima i vektor u koji
     // se spremaju indeksi svih pojavljivanja.
     // Vraca indeks prvog pojavljivanja unutar niza.
     static int all_occurrences(char* str, int n, bool* mask, int m, char *pattern, int n_pattern, int* SA_masked, std::vector<int>& v);
     
     static int first_occurrence(char* str, int n, bool* mask, int m, char *pattern, int n_pattern, int* SA_masked, std::vector<int>& v);

 private:
     static int cmp(char *str, bool *mask, int m, char *pattern, int n_pattern);
};