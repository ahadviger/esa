#pragma once

#include <vector>

#include "lcpinterval.hpp"
#include "childtable.hpp"

class IPM {
 public:  
     // Vraca maskirano sufiksno polje.
     // Prima niz, duljinu niza, masku i duljinu maske.
     static int* get_masked_SA(char* _str, int n, bool* mask, int m);

     // Trazi sva pojavljivanja znakovnog niza pattern pod maskom mask
     // unutar niza nad kojim je izgradjeno polje.
     // Vraca broj pojavljivanja unutar niza i u varijablu idx upisuje
     // indeks prvog pojavljivanja.
     static int all_occurrences(char* str, int n, bool* mask, int m, char *pattern, int n_pattern, int* SA_masked, int* idx);
     
 private:
     static int cmp(char *str, bool *mask, int m, char *pattern, int n_pattern);
};
