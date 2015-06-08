#pragma once

// Sadrzi informacije o LCP-intervalu.
class LcpInterval {
 public:
     // LCP-vrijednost intervala.
     int lcp;
     // lijeva granica intervala
     int Lb; 
     // desna granica intervala
     int Rb;
     LcpInterval() {}
     LcpInterval(int _lcp, int _Lb, int _Rb) {
         lcp = _lcp; Lb = _Lb, Rb = _Rb;
     }
};