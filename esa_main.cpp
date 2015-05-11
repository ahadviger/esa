#include <cstdio>
#include <cstring>
#include <algorithm>
#include <stack>
#include <vector>
#include "lcpinterval.hpp"
#include "childtable.hpp"
#include "esa.hpp"

int main(void) {
	
	char S[10000];
	scanf("%s", S);
	
	ESA* esa = new ESA(S);
	esa->print_sorted();
	
	return 0;
}