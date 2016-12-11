#include  <cstdio>
#include "Integer.h"
#include <time.h>

int main()
{
	Integer P, Q, N;
	int bits = 1024;
	clock_t start, end;
	start = clock();
	GenerateKeys(bits, P, Q, N);
	end = clock();
	printf("Time Cost: %.6lfs\n", (double)(end-start)/CLOCKS_PER_SEC);
	P.PrintHex();
	Q.PrintHex();
	return 0;
}
