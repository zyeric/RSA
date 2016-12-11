#include "Integer.h"
#include <cstdlib>

int main(int argc, char* argv[])
{
	if (argc == 3)
	{
		//生成密钥
		//输出结果分别为 P Q N E D 每行一个
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
