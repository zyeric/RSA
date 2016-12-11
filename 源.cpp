#include  <cstdio>
#include "Integer.h"
#include <time.h>

int main()
{
	Integer P, Q, N, E, D;
	int bits = 768;
	clock_t start, end;
	start = clock();
	GenerateKeys(bits, P, Q, N, E, D);
	end = clock();
	printf("Time Cost: %.6lfs\n", (double)(end-start)/CLOCKS_PER_SEC);
	Integer Phi = N.Sub(P).Sub(Q);
	Phi.Add1();

	Integer x = Integer(bits-1);
	Integer t = Encrypt(x, E, N);
	Integer y = Decrypt(t, D, N);

	int cmpRes = x.CompareTo(y);
	if (cmpRes == 0)
	{
		printf("Correct\n");
	}
	else
	{
		printf("Wrong");
	}
	return 0;
}
