#pragma once

#include <vector>

#include "childtable.hpp"

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
     int search(char *p, int m, std::vector<int>& v);
     
     // Trazi sva sufiks-prefiks preklapanja, odnosno sve sufikse
     // niza nad kojim je izgradjeno polje koji su ujedno prefiksni
     // zadanog niza p duljine m. Prima i vektor u koji se
     // spremaju duljine svih preklapanja.
     // Vraca duljinu maksimalnog sufiks-prefiks preklapanja.
     int overlap(char *p, int m, std::vector<int>& v);
     
     // Vraca indeks prvog pojavljivanja niza p unutar
     // niza nad kojim je izgradjeno polje, no za pretragu
     // koristi samo sufiksno polje.
     int search_SA(char *p, int m);

     // Ispisuje sortirane sufikse.
     void print_sorted();
};
