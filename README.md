# Enhanced Suffix Array

Implementacija poboljšanog sufiksnog polja. Izgradnja sufiksnog polja algoritmom SA-IS.

1. izgradnja primjera za testiranje ESA (esa_main): `make esa`
(program kao argument prima putanju datoteke s referentnim genomom, sa standardnog ulaza prima duljinu traženog očitanja i samo očitanje)

2. izgradnja primjera za testiranje inexact pattern matchinga (ipm_main): `make ipm`
(program kao argument prima putanju datoteke s referentnim genomom)

##### Enhanced Suffix Array: esa.hpp
```c++
     // Konstruktor. Prima znakovni niz i njegovu duljinu
     // Ako je esa=true, gradi se poboljsano sufiksno polje, inace samo SA.
     // Na kraj niza dodaje se znak '|' veci od svih drugih znakova
     // abecede kako nijedan sufiks ne bi bio prefiks drugog sufiksa.
     ESA(char* _str, int _n, bool esa);
    
     // Vraca sufiksno polje.
     int* get_SA();

     // Vraca inverzno sufiksno polje.
     int* get_ISA();

     // Vraca polje najduljih zajednickih prefiksa.
     int* get_LCP();

     // Vraca tablicu djece.
     ChildTable* get_child_table();

     // Trazi sva pojavljivanja znakovnog niza p duljine m unutar
     // niza nad kojim je izgradjeno polje. Prima i vektor u koji
     // se spremaju indeksi svih pojavljivanja.
     // Vraca indeks prvog pojavljivanja unutar niza.
     int all_occurrences(char *p, int m, std::vector<int>& v);
     
     // Provjerava nalazi li se niz p unutar niza nad kojim je
     // izgradjeno polje.
     bool contains(char *p, int m);
     
     // Trazi sva sufiks-prefiks preklapanja, odnosno sve sufikse
     // niza nad kojim je izgradjeno polje koji su ujedno prefiksi
     // zadanog niza p duljine m. Prima i vektor u koji se
     // spremaju duljine svih preklapanja.
     // Vraca duljinu maksimalnog sufiks-prefiks preklapanja.
     int overlap(char *p, int m, std::vector<int>& v);
     
     // Trazi sva prefiks-sufiks preklapanja, odnosno sve prefikse
     // niza nad kojim je izgradjeno polje koji su ujedno sufiksi
     // zadanog niza p duljine m. Prima i vektor u koji se
     // spremaju duljine svih preklapanja.
     // Vraca duljinu maksimalnog prefiks-sufiks preklapanja.
     int overlap_reverse(char *p, int m, std::vector<int>& v);
     
     // Trazi sva prefiks-sufiks preklapanja, odnosno sve prefikse/sufikse
     // niza nad kojim je izgradjeno polje koji su ujedno sufiksi/prefiksi
     // zadanog niza p duljine m. Prima i 2 vektora u koji se
     // spremaju duljine svih preklapanja (u prvi pref-suf, a u drugi suf-pref).
     // Vraca duljinu maksimalnog suf-pref/pref-suf preklapanja.
     int all_overlaps(char *p, int m, std::vector<int>& v1, std::vector<int>& v2);

     // Vraca indeks nekog pojavljivanja niza p unutar
     // niza nad kojim je izgradjeno polje, no za pretragu
     // koristi samo sufiksno polje.
     int search_SA(char *p, int m);

     // Ispisuje sortirane sufikse.
     void print_sorted();
```

##### Inexact Pattern Matching: ipm.hpp
```c++
     // Vraca maskirano sufiksno polje ulaznog niza _str.
     // Prima niz, duljinu niza n, masku i duljinu maske m.
     static int* get_masked_SA(char* _str, int n, bool* mask, int m);

     // Trazi sva pojavljivanja znakovnog niza pattern pod maskom mask duljine m
     // unutar niza str duljine n. Prima i maskirano sufiksno polje SA_masked.
     // Vraca broj pojavljivanja unutar niza i u varijablu idx upisuje
     // indeks prvog pojavljivanja unutar sufiksnog polja.
     static int all_occurrences(char* str, int n, bool* mask, int m, char *pattern, int n_pattern, int* SA_masked, int* idx);
```     
