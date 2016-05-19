#pragma once

#include <vector>

#include "lcpinterval.hpp"
#include "childtable.hpp"

// Klasa koja izgradjuje poboljsano sufiksno polje, odnosno: 
// sufiksno polje, inverzno sufiksno polje, polje najduljih 
// zajednickih prefiksa i tablicu djece.
class ESA {
 public:
     // Konstruktor. Prima znakovni niz i njegovu duljinu te za taj niz
     // izgradjuje poboljsano sufiksno polje.
     // Na kraj niza dodaje se znak '|' veci od svih drugih znakova
     // abecede kako nijedan sufiks ne bi bio prefiks drugog sufiksa.
     ESA(char* _str, int _n);
    
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

     // Vraca indeks prvog pojavljivanja niza p unutar
     // niza nad kojim je izgradjeno polje, no za pretragu
     // koristi samo sufiksno polje.
     int search_SA(char *p, int m);

     // Ispisuje sortirane sufikse.
     void print_sorted();

 private:
     char *str;
     int n, *S, *SA;
     int *LCP, *ISA;
     ChildTable *childtab;

     // Odredjuje je li pojedini znak niza L-tip ili S-tip znaka.
     void determine_char_type(int *S, int n, bool *char_type);

     // U niz sprema indekse svih LMS-znakova unutar niza.
     void determine_LMS(bool *char_type, int n, int *LMS, int *n_LMS);

     // Provjerava je li znak na i-tom indeksu LMS-znak.
     bool is_LMS(bool *char_type, int i);

     // Postavlja pokazivace na krajeve (B) i pocetke potpolja (B_start).
     void initialize_buckets(int *S, int n, int *B, int *B_start);

     // Inducirano sortiranje LMS-podnizove.
     void induced_sort(int *SA, int *S, int *B, int *B_start, int *B_end, int n, int *LMS, int n_LMS, bool *char_type);

     // Oduzimanje najmanje vrijednosti od svih clanova niza (najmanja postaje 1).
     void normalize_string();

     // Provjerava jesu li dva LMS-podniza jednaka.
     bool compare_LMS(int *S, bool *char_type, int n, int n1, int n2);

     // Imenovanje LMS-podnizova.
     void name_substrings(int *S1, int *SA, int *S, int *LMS, int n, int n_LMS, bool *char_type, bool *unique);

     // Odredjivanje SA iz SA1.
     void determine_SA(int *SA, int *S, int *SA1, int *LMS, int *B, int *B_start, int *B_end, bool *char_type, int n, int n_LMS);

     // Odredjivanje sufiksnog polja.
     void solve_SA(int *S, int *SA, int n, int *B, int *B_start, int *B_end);

     // Odredjivanje inverznog sufiksnog polja.
     void determine_ISA();

     // Odredjivanje polja najduljih zajednickih prefiksa.
     void determine_LCP();

     // Odredjivanje up, down i nextlIndex vrijednosti tablice djece.
     void construct_child_table();

     // Vraca LCP-vrijednost intervala [i..j].
     int getlcp(int i, int j);

     // Trazi dijete LCP-intervala [i..j] ciji sufiksi imaju
     // znak a na l-tom mjestu gdje je l LCP-vrijednost tog intervala.
     LcpInterval* getInterval(int i, int j, char a);
     
     // Trazi i vraca interval svih pojavljivanja niza p unutar niza.
     std::pair<int, int> search(char *p, int m);
};