#pragma once

class ChildTable {
	public:
		int up, down, nextlIndex;
		ChildTable() {}
		ChildTable(int _up, int _down, int _nextlIndex) {
			up = _up; down = _down; nextlIndex = _nextlIndex;
		}
};