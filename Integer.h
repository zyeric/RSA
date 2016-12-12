typedef unsigned int uint;
typedef unsigned long long ull;

const int BIT = 24;
const int BITMAX = 1<<BIT;
const int BITMAXMASK = (1<<BIT)-1;
const int MAXLENGTH = 1024/BIT+1;
const int MILLERTIME = 20;

class Integer
{
public:
	//length表示A数组保存内容的长度 不是二进制位的长度
	int length;
	//二进制位的长度
	int binLength;
	//考虑到做乘法位数的扩张 存储为两倍空间
	uint A[MAXLENGTH<<1];
	bool isZero;

	//默认构造函数
	Integer();

	Integer(char *s);
	//产生固定位长的奇数
	Integer(int _);

	static Integer ONE();
	static Integer ZERO();
	static Integer Power2(int n);

	Integer ShiftRight(int n);
	Integer ShiftLeft(int n);

	bool IsOne();
	
	void Add1();
	void Add2();
	Integer Add(Integer p);

	Integer Sub1();
	//n-1=d*2^s
	//返回d
	Integer SubDecompose();
	//假设p小于当前数
	Integer Sub(Integer p);

	//将比第n位高的数全设为0 包括第n位
	//假设n > 0
	//实际上是求 mod 2^n的值
	Integer Remove(int n);

	//做乘法
	Integer Multiply(Integer p);

	//返回1表示当前数大
	//返回0表示相等
	//返回-1表示p大
	int CompareTo(Integer p);

	void PrintDecimal();
	void PrintHex();
};

//N 是 bits位的 P和Q是bits/2位的素数
extern void GenerateKeys(int bits, Integer &P, Integer &Q, Integer &N, Integer &E, Integer &D);
extern Integer PowMod(Integer a, Integer n, Integer p);