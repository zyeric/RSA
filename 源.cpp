#include "Integer.h"
#include <cstdlib>
#include <time.h>
#include <cstdio>

int main(int argc, char* argv[])
{
	clock_t start, end;
	start = clock();
	if (argc == 3)
	{
		//������Կ
		//�������ֱ�Ϊ P Q N E D ÿ��һ��
		int bits = atoi(argv[2]);
		Integer N, P, Q, E, D;
		GenerateKeys(bits, P, Q, N, E, D);
		P.PrintHex();
		Q.PrintHex();
		N.PrintHex();
		E.PrintHex();
		D.PrintHex();
	}
	else
	{
		Integer a = Integer(argv[2]);
		Integer b = Integer(argv[3]);
		Integer c = Integer(argv[4]);
		Integer d = PowMod(a, b, c);
		d.PrintHex();
	}
	end = clock();
	printf("Time Cost: %.3lfs\n", (double)(end-start)/CLOCKS_PER_SEC);
	return 0;
}
