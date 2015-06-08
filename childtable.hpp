#pragma once

// Klasa modelira jedan redak tablice djece.
class ChildTable {
 public:
     // up vrijednost tablice
     int up;
     // down vrijednost tablice
     int down;
     // nextlIndex vrijednost tablice
     int nextlIndex;
     ChildTable() {}
     ChildTable(int _up, int _down, int _nextlIndex) {
         up = _up; down = _down; nextlIndex = _nextlIndex;
     }
};