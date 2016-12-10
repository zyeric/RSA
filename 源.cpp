#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <time.h>
#include <algorithm>

typedef unsigned int uint;
typedef unsigned long long ull;

const int BIT = 24;
const int BITMAX = 1<<BIT;
const int MAXLENGTH = 1024/BIT+1;
const int MILLERTIME = 20;

const int PRIMECOUNT = 50;
int Primes[PRIMECOUNT];
int Residual[PRIMECOUNT];

ull Buffer[MAXLENGTH<<1];

//struct的成员都是public类型

struct Integer
{
	//length表示A数组保存内容的长度 不是二进制位的长度
	int length;
	//二进制位的长度
	int binLength;
	//考虑到做乘法位数的扩张 存储为两倍空间
	uint A[MAXLENGTH<<1];
	bool isZero;

	void Initial(char * s)
	{
		memset(A,0,sizeof(A));
		binLength = int(strlen(s));
		isZero = false;
		int now = BIT/4;
		for (int i = 0; i < binLength; ++i)
		{
			char cur = s[binLength-1-i];
			int val = cur >= 'A' ? cur-'A'+10:cur-'0';
			A[i/now] |= val << (4*(i%now));
		}
		binLength *= 4;
		length = binLength % BIT == 0 ? binLength/BIT:(binLength/BIT+1);
	}

	void Initial()
	{
		memset(A, 0, sizeof(A));
		isZero = false;
		binLength = 0;
		length = 0;
	}

	//初始化为_长度的二进制数
	//假设输入长度为BIT的倍数
	void Initial(int _)
	{
		memset(A, 0, sizeof(A));
		isZero = false;
		binLength = _;
		length = _/BIT;
		if (_ % BIT != 0)	length++;
		
		//保证是一个奇数
		while (true)
		{
			A[0] = rand()%BITMAX;
			if (A[0] & 1)
			{
				break;
			}
			else
			{
				continue;
			}
		}

		for (int i = 1; i < length; ++i)
		{
			A[i] = rand()%BITMAX;
		}

		//把多余的位消掉
		for (int i = BIT - 1; i >= 0; --i)
		{
			int now = (length-1)*BIT + i;
			if (now >= binLength)
			{
				if (A[length-1] & (1<<i))
				{
					A[length-1] ^= (1<<i);
				}
			}
			else if (now == binLength - 1)//保证最高位是1
			{
				A[length-1] |= (1<<i);
				break;
			}
		}
	}

	Integer ShiftLeft(int n)
	{
		Integer res;
		res.Initial();
		for (int i = 0; i < length; ++i)
		{
			for (int j = 0; j < BIT; ++j)
			{
				if (A[i] & (1 << j))
				{
					int now = i * BIT + j;
					int nxt = now + n;
					res.A[nxt/BIT] |= (1<<(nxt%BIT));
				}
			}
		}

		//维护length和binLength
		res.binLength = binLength + n;
		res.length = res.binLength % BIT == 0 ? res.binLength/BIT : (res.binLength/BIT+1);

		return res;
	}

	Integer ShiftRight(int n)
	{
		Integer res;
		res.Initial();

		bool flag = false;
		for (int i = length - 1; i >= 0; --i)
		{
			for (int j = BIT-1; j >= 0; --j)
			{
				if (A[i] & (1 << j))
				{
					int now = i * BIT + j;
					int nxt = now - n;
					if (nxt < 0)
					{
						flag = true;
						break;
					}
					else
					{
						res.A[nxt/BIT] |= (1<<(nxt%BIT));
					}
				}
			}
			if (flag)	break;
		}

		//维护length和binLength
		res.binLength = binLength - n;
		res.length = res.binLength % BIT == 0 ? res.binLength/BIT : (res.binLength/BIT+1);

		return res;
	}

	bool IsOne()
	{
		return (binLength == 1) && (A[0] == 1);
	}

	//自增1
	void Add1()
	{
		for (int i = 0; i < length; ++i)
		{
			A[i]++;
			if (A[i] < BITMAX)
			{
				break;
			}
			else
			{
				A[i] = 0;
			}
		}
	}

	//自增2
	void Add2()
	{
		Add1();
		Add1();
	}

	//自减1
	Integer Sub1()
	{
		Integer res;
		res.Initial();
		for (int i = 0; i < length; ++i)
		{
			res.A[i] = A[i];
		}
		for (int i = 0; i < length; ++i)
		{
			if (res.A[i] == 0)
			{
				res.A[i] = BITMAX-1;
			}
			else
			{
				res.A[i]--;
				break;
			}
		}
		if (res.A[length-1] == 0)
		{
			res.length = length - 1;
		}
		else
		{
			res.length = length;
		}

		res.binLength = (res.length-1) * BIT;
		for (int i = BIT-1; i >= 0; --i)
		{
			if (res.A[res.length-1] & (1<<i))
			{
				res.binLength += i + 1;
				break;
			}
		}
		return res;
	}

	//n-1=d*2^s
	//返回d
	Integer SubDecompose()
	{
		Integer tmp = Sub1();
		int n = 0;
		for (int i = 0; i < tmp.length; ++i)
		{
			if (tmp.A[i] != 0)
			{
				for (int j = 0; j < BIT; ++j)
				{
					if (tmp.A[i] & (1<<j))
					{
						break;
					}
					else
					{
						n ++;
					}
				}
				break;
			}
			else
			{
				n += BIT;
			}
		}

		return tmp.ShiftRight(n);
	}

	//假设p小于当前数
	Integer Sub(Integer p)
	{
		Integer res;
		res.Initial();
		int own = 0;
		for (int i = 0; i < length; ++i)
		{
			if (A[i] < p.A[i] + own)
			{
				res.A[i] = A[i] + BITMAX - own - p.A[i];
				own = 1;
			}
			else
			{
				res.A[i] = A[i] - own - p.A[i];
				own = 0;
			}
		}

		//修改了状态 维护length和binLength的值
		for (int i = length - 1; i >= 0; --i)
		{
			if (res.A[i] != 0)
			{
				res.length = i + 1;
				res.binLength = BIT * i;
				for (int j = BIT - 1; j >= 0; --j)
				{
					if (res.A[i] & (1 << j))
					{
						res.binLength += j + 1;
						break;
					}
				}
				break;
			}
		}

		return res;
	}

	//做加法
	Integer Add(Integer p)
	{
		Integer ret;
		ret.Initial();

		int bound = std::max(length, p.length);

		for (int i = 0; i < bound; ++i)
		{
			ret.A[i] = A[i] + p.A[i];
		}

		int carry = 0;
		for (int i = 0; i < bound; ++i)
		{
			ret.A[i] = ret.A[i] + carry;

			if (ret.A[i] >= BITMAX)
			{
				carry = 1;
				ret.A[i] -= BITMAX;
			}
			else
			{
				carry = 0;
			}
		}

		if (carry > 0)
		{
			ret.A[bound] = carry;
			bound ++;
		}

		ret.length = bound;

		ret.binLength = (ret.length-1)*BIT;

		for (int i = BIT - 1; i >= 0; --i)
		{
			if (ret.A[ret.length-1] & (1<<i))
			{
				ret.binLength += i + 1;
				break;
			}
		}

		return ret;
	}

	//将比第n位高的数全设为0 包括第n位
	//假设n > 0
	//实际上是求 mod 2^n的值
	Integer Remove(int n)
	{
		Integer ret;
		ret.Initial();

		for (int i = 0; i < length; ++i)
		{
			ret.A[i] = A[i];
		}

		for (int i = n; i < binLength; ++i)
		{
			int x = i / BIT;
			int y = i % BIT;
			if (ret.A[x] & (1<<y))
			{
				ret.A[x] ^= (1<<y);
			}
		}

		for (int i = length -1; i >= 0; --i)
		{
			if (ret.A[i] > 0)
			{
				ret.length = i+1;
				break;
			}
		}

		ret.binLength = BIT*(ret.length-1);
		for (int i = BIT-1; i >= 0; --i)
		{
			if (ret.A[ret.length-1] & (1<<i))
			{
				ret.binLength += i+1;
				break;
			}
		}

		return ret;
	}

	//做乘法
	Integer Multiply(Integer p)
	{
		Integer res;
		res.Initial();

		for (int i = 0; i < length + p.length; ++i)
		{
			Buffer[i] = 0;
		}

		for (int i = 0; i < length; ++i)
		{
			for (int j = 0; j < p.length; ++j)
			{
				Buffer[i+j] += 1ll * A[i] * p.A[j];
			}
		}

		ull carry = 0;
		for (int i = 0; i < length + p.length; ++i)
		{
			Buffer[i] += carry;
			if (Buffer[i] >= BITMAX)
			{
				carry = Buffer[i] / BITMAX;
				Buffer[i] = Buffer[i] % BITMAX;
			}
			else
			{
				carry = 0;
			}
		}

		if (Buffer[length+p.length-1] != 0)
		{
			res.length = length + p.length;
		}
		else
		{
			res.length = length + p.length - 1;
		}

		for (int i = 0; i < res.length; ++i)
		{
			res.A[i] = Buffer[i];
		}

		res.binLength = BIT*(res.length-1);
		for (int i = BIT-1; i >= 0; --i)
		{
			if (res.A[res.length-1] & (1<<i))
			{
				res.binLength += i + 1;
				break;
			}
		}

		return res;
	}

	//返回1表示当前数大
	//返回0表示相等
	//返回-1表示p大
	int CompareTo(Integer p)
	{
		if (binLength < p.binLength)
		{
			return -1;
		}
		if (binLength > p.binLength)
		{
			return 1;
		}
		//若binLength相等 则length必然相等
		for (int i = length - 1; i >= 0; --i)
		{
			if (A[i] > p.A[i])
			{
				return 1;
			}
			if (A[i] < p.A[i])
			{
				return -1;
			}
		}
		return 0;
	}

	void PrintDecimal()
	{
		for (int i = length - 1; i >= 0; --i)
		{
			printf("%d ", A[i]);
		}
		printf("\n");
	}

	void PrintHex()
	{
		if (isZero)
		{
			printf("0");
		}
		for (int i = length - 1; i >= 0; --i)
		{
			printf("%X ", A[i]);
		}
		printf("\n");
	}
};

//返回p mod q
Integer Mod(Integer p, Integer q)
{
	while (true)
	{
		int cmpRes = p.CompareTo(q);
		if (cmpRes == -1)
		{
			break;
		}
		if (cmpRes == 0)
		{
			Integer zero;
			zero.Initial();
			zero.isZero = true;
			return zero;
		}

		//p>q且二进制位数相等
		if (p.binLength == q.binLength)
		{
			return p.Sub(q);
		}

		Integer tmp = q.ShiftLeft(p.binLength - q.binLength);
		int cmpRes2 = p.CompareTo(tmp);
		if (cmpRes2 == 0)
		{
			Integer zero;
			zero.Initial();
			zero.isZero = true;
			return zero;
		}
		else
		{
			if (cmpRes2 < 0)
			{
				p = p.Sub(q.ShiftLeft(p.binLength - q.binLength - 1));
			}
			else
			{
				p = p.Sub(tmp);
			}
		}
	}

	/*bool flag1 = false;
	bool flag2 = false;

	for (int i = 0; i < p.length; ++i)
	{
		if (p.A[i] >= BITMAX)
		{
			flag1 = true;
		}
	}

	for (int i = 0; i < q.length; ++i)
	{
		if (q.A[i] >= BITMAX)
		{
			flag2 = true;
		}
	}

	if (flag1 || flag2)
	{
		int val = 38;
		val ++;
	}*/

	return p;
}

//返回GCD(p,q)
Integer GCD(Integer p, Integer q)
{
	/*if (q.isZero == true)
	{
		return p;
	}
	else
	{
		return GCD(q, Mod(p,q));
	}*/
	Integer tmp;
	while (q.isZero == false)
	{
		tmp = q;
		q = Mod(p,q);
		p = tmp;
	}
	return p;
}

//返回a^n mod p
Integer PowMod(Integer a, Integer n, Integer p)
{
	Integer ret;
	ret.Initial();
	ret.binLength = 1;
	ret.length = 1;
	ret.A[0] = 1;

	for (int i = 0; i < n.binLength; ++i)
	{
		if (n.A[i/BIT] & (1<<(i%BIT)))
		{
			ret = Mod(ret.Multiply(a), p);
			//printf("PowMod: ");
			//ret.PrintHex();
		}
		Integer tmp = a.Multiply(a);
		a = Mod(tmp, p);
		//printf("PowMod a: ");
		//a.PrintHex();
	}

	return ret;
}

//求p关于2^bits的逆元
//p保证是奇数
//所以p^{2^{bits-1}}一定是1
Integer Inv(Integer p, int bits)
{
	Integer cur = p;

	for(;;)
	{
		p = p.Multiply(p);
		if (p.binLength > bits)
		{
			p = p.Remove(bits);
		}
		if (p.IsOne())
		{
			break;
		}
		else
		{
			cur = cur.Multiply(p);
			if (cur.binLength > bits)
			{
				cur = cur.Remove(bits);
			}
		}
	}

	return cur;
}

Integer MontgomeryMultiply(Integer x, Integer y, Integer p, int bits, Integer N)
{
	Integer t = x.Multiply(y);
	Integer tmp = t.Remove(bits);
	Integer u = tmp.Multiply(N).Remove(bits);
	Integer tmp2 = u.Multiply(p);
	Integer r = t.Add(tmp2);
	r = r.ShiftRight(bits);
	
	/*if (r.binLength > bits)
	{
		r = r.ShiftRight(bits);
	}
	else
	{
		int val = 38;
		val ++;
	}*/


	if (r.CompareTo(p) >= 0)
	{
		r = r.Sub(p);
	}

	return r;
}

Integer MontgomeryPowMod(Integer a, Integer n, Integer p, int bits, Integer R, Integer N)
{
	a = Mod(a.Multiply(R), p);
	Integer ret = Mod(R, p);

	for (int i = 0; i < n.binLength; ++i)
	{
		if (n.A[i/BIT] & (1<<(i%BIT)))
		{
			ret = MontgomeryMultiply(ret, a, p, bits, N);
		}
		a = MontgomeryMultiply(a, a, p, bits, N);
	}

	//现在返回的是 a^n * R mod p
	return ret;
}

int cnt;

bool MongomeryMillar_Rabin(Integer p)
{
	cnt ++;
	int bits = p.binLength;
	Integer d = p.SubDecompose();
	Integer tmp = p.Sub1();
	int s = p.binLength - d.binLength;

	Integer R;
	R.Initial();
	R.binLength = bits+1;
	R.length = R.binLength % BIT == 0 ? R.binLength / BIT:(R.binLength/BIT+1);
	R.A[bits/BIT] ^= 1<<(bits%BIT);

	Integer N = Inv(p, bits);
	N = R.Sub(N);

	Integer x = Mod(R, p);
	Integer y = Mod(x.Multiply(tmp), p);

	for (int i = 0; i < MILLERTIME; ++i)
	{
		Integer a;
		a.Initial(p.binLength);
		a.A[0] ^= rand()&1;
		int cmpRes = p.CompareTo(a);

		if (cmpRes == 0)
		{
			i--;
			continue;;
		}
		if (cmpRes == -1)
		{
			a = Mod(a,p);
		}
		if (a.IsOne())
		{
			i --;
			continue;
		}
		Integer t = GCD(a, p);
		if (t.binLength > 1)
		{
			return false;
		}
		//Integer ad = PowMod(a,d,p);
		Integer ad = MontgomeryPowMod(a, d, p, bits, R, N);

		if (ad.CompareTo(x) == 0)
		{
			continue;
		}
		else
		{
			bool flag = false;
			for (int j = 0; j < s; ++j)
			{
				if (ad.CompareTo(y) == 0)
				{
					flag = true;
					break;
				}
				else
				{
					//ad = Mod(ad.Multiply(ad), p);
					ad = MontgomeryMultiply(ad, ad, p, bits, N);
				}
			}

			if (!flag)
			{
				return false;
			}
		}
	}

	return true;
}

bool Miller_Rabin(Integer p)
{
	cnt++;
	Integer tmp = p.Sub1();
	Integer d = p.SubDecompose();
	int s = p.binLength - d.binLength;

	for (int i = 0; i < MILLERTIME; ++i)
	{
		Integer a;
		a.Initial(p.binLength);
		a.A[0] ^= rand()&1;
		if (p.CompareTo(a) == -1)
		{
			a = Mod(a,p);
		}
		if (a.IsOne())
		{
			i --;
			continue;
		}
		Integer t = GCD(a, p);
		if (t.binLength > 1)
		{
			return false;
		}
		Integer ad = PowMod(a,d,p);

		if (ad.IsOne())
		{
			continue;
		}
		else
		{
			bool flag = false;
			for (int j = 0; j < s; ++j)
			{
				if (ad.CompareTo(tmp) == 0)
				{
					flag = true;
					break;
				}
				else
				{
					ad = Mod(ad.Multiply(ad), p);
				}
			}

			if (!flag)
			{
				return false;
			}
		}
	}

	return true;
}

void BasicTest()
{
	//检查减法和比较大小
	Integer a,b;
	a.Initial(32);
	b.Initial(32);
	a.PrintHex();
	b.PrintHex();
	if (a.CompareTo(b) == 1)
	{
		a.Sub(b).PrintHex();
	}
	else
	{
		b.Sub(a).PrintHex();
	}

	Integer c,d;
	c.Initial(32);
	d.Initial(48);
	c.PrintHex();
	d.PrintHex();
	Mod(d,c).PrintHex();
	Mod(d,d.ShiftRight(2));
}

void MultiplyTest()
{
	Integer a,b;
	a.Initial(16);
	b.Initial(16);
	a.PrintDecimal();
	b.PrintDecimal();
	Integer tmp = a.Multiply(a);
	Mod(tmp, b).PrintDecimal();
}

void ModTest()
{
	char s[9] = {'9', '3', '8', '0', 'F', '8', '4', '3', '\0'};
	char t[9] = {'A', '0', 'E', '2', '6', '9', 'F', 'F', '\0'};
	Integer a, p;
	a.Initial(s);
	p.Initial(t);
	Integer tmp = a.Multiply(a);
	tmp.PrintHex();
	Mod(a.Multiply(a), p).PrintHex();
}

void PowModTest()
{
	char s[9] = {'5', '6', 'E', 'B', 'A', '7', '1', '7', '\0'};
	char t[9] = {'A', '0', 'E', '2', '6', '9', 'F', 'F', '\0'};
	char u[9] = {'5', '0', '7', '1', '3', '4', 'F', 'F', '\0'};
	Integer a, p, d;
	a.Initial(s);
	p.Initial(t);
	d.Initial(u);
	Integer tmp = a.Multiply(a);
	tmp.PrintHex();
	Mod(a.Multiply(a), p).PrintHex();
	PowMod(a, d, p).PrintHex();
}

void GCDTest()
{
	Integer a;
	a.Initial(256);
	a.PrintHex();
	GCD(a,a.ShiftRight(1)).PrintHex();
}

void MillerInitial()
{
	int now = 0;
	for (int i = 3; ; ++i)
	{
		bool flag = true;
		for (int j = 2; j*j <= i; ++j)
		{
			if (i % j == 0)
			{
				flag = false;
				break;
			}
		}

		if (flag)
		{
			Primes[now ++] = i;
		}

		if (now == PRIMECOUNT)
		{
			break;
		}
	}

	for (int i = 0; i < PRIMECOUNT; ++i)
	{
		Residual[i] = BITMAX % Primes[i];
	}
}

int mod[PRIMECOUNT];

void MillerTest(int n)
{
	Integer a;
	a.Initial(n);
	//a.PrintDecimal();
	//a.SubDecompose().PrintDecimal();

	memset(mod, 0, sizeof(mod));

	for (int i = 0; i < PRIMECOUNT; ++i)
	{
		int cur = 1;
		for (int j = 0; j < a.length; ++j)
		{
			mod[i] = mod[i] + cur*a.A[j];
			cur = cur * Residual[i] % Primes[i];
		}
		mod[i] %= Primes[i];
	}

	while (true)
	{
		bool flag = true;
		for (int i = 0; i < PRIMECOUNT; ++i)
		{
			if (mod[i] == 0)
			{
				flag = false;
				break;
			}
		}
		if (flag && MongomeryMillar_Rabin(a))
		{
			a.PrintHex();
			break;
		}
		else
		{
			a.Add2();
			for (int i = 0; i < PRIMECOUNT; ++i)
			{
				mod[i] = (mod[i]+2)%Primes[i];
			}
		}
	}

	/*char s[9] = {'A','0','E','2','6','9','F','F', '\0'};
	Integer a;
	a.Initial(s);
	a.PrintHex();

	printf("%d\n", Miller_Rabin(a));*/
}

void GeneratePrime()
{
	MillerInitial();
	clock_t start, end;
	int n = 48;
	int steps = 10;
	double averageTime = 0;
	for (int i = 0; i < steps; ++i)
	{
		//执行Miller-Rabin的次数
		cnt = 0;
		start = clock();
		MillerTest(n);
		end = clock();
		double curTime = (double)(end-start) / CLOCKS_PER_SEC;
		printf("%.6lfs    ", curTime);
		printf("Miller-Rabin: %d\n", cnt);
		averageTime += curTime;
	}
	printf("%.6lf\n", averageTime/steps);
}

void MontgomeryMultiplyTest()
{
	char s[2] = {'A', '\0'};
	char t[2] = {'4', '\0'};
	char u[2] = {'B', '\0'};
	char v[2] = {'D', '\0'};

	Integer x, y, p, N;
	x.Initial(s);
	y.Initial(t);
	p.Initial(u);
	N.Initial(v);

	Integer ret = MontgomeryMultiply(x, y, p, 4, N);

	ret.PrintHex();
}

void InvTest()
{
	char s[3] = {'1', 'F', '\0'};

	Integer a;
	a.Initial(s);

	Inv(a, 5).PrintHex();
}

void MontgomeryMiller_RabinTest()
{
	/*char s[13] = {'8', '0', '5', '1', 'F', 'D', '0', '0', '3', '5', '7', '1', '\0'};
	
	Integer a;
	a.Initial(s);

	if (MongomeryMillar_Rabin(a) == true)
	{
		printf("OK");
	}
	else
	{
		printf("Error");
	}*/

	//while (true)
	//{
	//	Integer a;
	//	a.Initial(384);

	//	if (MongomeryMillar_Rabin(a) == true)
	//	{
	//		a.PrintHex();
	//		break;
	//	}
	//}

	MillerInitial();
	clock_t start, end;
	int n = 384;
	int steps = 10;
	double averageTime = 0;
	for (int i = 0; i < steps; ++i)
	{
		//执行Miller-Rabin的次数
		cnt = 0;
		start = clock();
		MillerTest(n);
		end = clock();
		double curTime = (double)(end-start) / CLOCKS_PER_SEC;
		printf("%.6lfs    ", curTime);
		printf("Miller-Rabin: %d\n", cnt);
		averageTime += curTime;
	}
	printf("%.6lf\n", averageTime/steps);
}

int main()
{
	srand((unsigned)time(NULL));
	
	//InvTest();
	MontgomeryMiller_RabinTest();
	//MontgomeryMultiplyTest();
	
	//BasicTest();
	//GCDTest();
	//MultiplyTest();
	
	//PowModTest();
	//ModTest();

	//GeneratePrime();
	return 0;
}
