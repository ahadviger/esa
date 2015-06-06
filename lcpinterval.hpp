#pragma once

class LcpInterval {
 public:
     int lcp, Lb, Rb;
     LcpInterval() {}
     LcpInterval(int _lcp, int _Lb, int _Rb) {
         lcp = _lcp; Lb = _Lb, Rb = _Rb;
     }
};