#include "Integer.h"
#include <cstdlib>

int main(int argc, char* argv[])
{
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
	return 0;
}
