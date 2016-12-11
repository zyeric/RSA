#include "Integer.h"
#include <time.h>
#include <cstring>
#include <cstdlib>
#include <algorithm>

ull Buffer[MAXLENGTH<<1];

Integer::Integer()
{
	memset(A, 0, sizeof(A));
	isZero = false;
	binLength = 0;
	length = 0;
}

Integer::Integer(char* s)
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

Integer::Integer(int _)
{
	memset(A, 0, sizeof(A));
	isZero = false;
	binLength = _;
	length = _/BIT;
	if (_ % BIT != 0)	length++;
		
	for (int i = 0; i < binLength; ++i)
	{
		if (rand() & 1)
		{
			A[i/BIT] |= 1<<(i%BIT);
		}
	}

	//保证是奇数
	A[0] |= 1;
	//保证最高位是1
	A[(binLength-1)/BIT] |= 1<<((binLength-1)%BIT);
}

Integer Integer::ONE()
{
	Integer ret;
	ret.binLength = 1;
	ret.length = 1;
	memset(ret.A, 0, sizeof(ret.A));
	ret.A[0] = 1;
	ret.isZero = false;
	return ret;
}

Integer Integer::ZERO()
{
	Integer ret;
	ret.binLength = 0;
	ret.length = 0;
	ret.isZero = true;
	memset(ret.A, 0, sizeof(ret.A));
	return ret;
}

Integer Integer::Power2(int n)
{
	Integer ret;
	ret.binLength = n+1;
	ret.length = ret.binLength % BIT == 0 ? ret.binLength/BIT : (ret.binLength/BIT+1);
	memset(ret.A, 0, sizeof(ret.A));
	ret.A[n/BIT] |= 1<<(n%BIT);
	ret.isZero = false;
	return ret;
}

Integer Integer::ShiftLeft(int n)
{
	Integer res = Integer();
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

Integer Integer::ShiftRight(int n)
{
	Integer res = Integer();

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

bool Integer::IsOne()
{
	return (binLength == 1) && (A[0] == 1);
}

void Integer::Add1()
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

void Integer::Add2()
{
	Add1();
	Add1();
}

Integer Integer::Add(Integer p)
{
	Integer ret= Integer();

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

Integer Integer::Sub1()
{
	Integer res = Integer();
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

Integer Integer::SubDecompose()
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

Integer Integer::Sub(Integer p)
{
	Integer res = Integer();
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

	if (res.length == 0 && res.binLength == 0)
	{
		res.isZero = true;
	}

	return res;
}

//将比第n位高的数全设为0 包括第n位
//假设n > 0
//实际上是求 mod 2^n的值
Integer Integer::Remove(int n)
{
	Integer ret = Integer();

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
Integer Integer::Multiply(Integer p)
{
	Integer res = Integer();

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
int Integer::CompareTo(Integer p)
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

void Integer::PrintDecimal()
{
	for (int i = length - 1; i >= 0; --i)
	{
		printf("%d ", A[i]);
	}
	printf("\n");
}

void Integer::PrintHex()
{
	if (isZero)
	{
		printf("0\n");
		return;
	}
	printf("%X", A[length-1]);
	for (int i = length - 2; i >= 0; --i)
	{
		printf("%06X", A[i]);
	}
	printf("\n");
}

Integer Divide(Integer p, Integer q)
{
	Integer res = Integer();

	while (true)
	{
		int cmpRes = p.CompareTo(q);
		if (cmpRes == -1)
		{
			break;
		}
		if (cmpRes == 0)
		{
			res.A[0] |= 1;
			break;
		}

		//若二进制位相等且p和q的位数相等 商加1
		if (p.binLength == q.binLength)
		{
			res.A[0] |= 1;
			break;
		}

		Integer tmp = q.ShiftLeft(p.binLength - q.binLength);
		int cmpRes2 = p.CompareTo(tmp);
		if (cmpRes2 == 0)
		{
			int now = p.binLength-q.binLength;
			res.A[now/BIT] |= 1<<(now%BIT);
			//res = res.Add(Integer::Power2(p.binLength-q.binLength));
			break;
		}
		else
		{
			if (cmpRes2 < 0)
			{
				int now = p.binLength - q.binLength - 1;
				res.A[now/BIT] |= 1<<(now%BIT);
				//res = res.Add(Integer::Power2(p.binLength - q.binLength - 1));
				p = p.Sub(q.ShiftLeft(p.binLength - q.binLength - 1));
			}
			else
			{
				int now = p.binLength-q.binLength;
				res.A[now/BIT] |= 1<<(now%BIT);
				//res = res.Add(Integer::Power2(p.binLength - q.binLength));
				p = p.Sub(tmp);
			}
		}
	}

	for (int i = p.length-1; i >= 0; --i)
	{
		if (res.A[i] != 0)
		{
			res.length = i+1;
			res.binLength = i*BIT;
			for (int j = BIT-1; j >= 0; --j)
			{
				if (res.A[i] & (1<<j))
				{
					res.binLength += 1+j;
					break;
				}
			}
			break;
		}
	}

	if (res.binLength == 0 && res.length == 0 && res.A[0] == 0)
	{
		res.isZero = true;
	}

	return res;
}

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
			return Integer::ZERO();
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
			return Integer::ZERO();
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

	return p;
}

//返回GCD(p,q)
Integer GCD(Integer p, Integer q)
{
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
	Integer ret = Integer::ONE();

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

bool MongomeryMillar_Rabin(Integer p)
{
	int bits = p.binLength;
	Integer d = p.SubDecompose();
	Integer tmp = p.Sub1();
	int s = p.binLength - d.binLength;

	Integer R = Integer::Power2(bits);

	Integer N = Inv(p, bits);
	N = R.Sub(N);

	Integer x = Mod(R, p);
	Integer y = Mod(x.Multiply(tmp), p);

	for (int i = 0; i < MILLERTIME; ++i)
	{
		Integer a = Integer(p.binLength);

		//抵消强制设为奇数的影响
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

bool Normal_Miller_Rabin(Integer p)
{
	Integer tmp = p.Sub1();
	Integer d = p.SubDecompose();
	int s = p.binLength - d.binLength;

	for (int i = 0; i < MILLERTIME; ++i)
	{
		Integer a = Integer(p.binLength);
		a.A[0] ^= rand()&1;
		int cmpRes = p.CompareTo(a);
		if (cmpRes == 0)
		{
			i --;
			continue;
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

const int PRIMECOUNT = 50;
int Primes[PRIMECOUNT];
int Residual[PRIMECOUNT];

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

Integer Montgomery_Generate_Prime(int n)
{
	Integer a = Integer(n);

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
			//a.PrintHex();
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

	return a;
}

void Extended_Sub(Integer a, int sign_a, Integer b, int sign_b, Integer& c, int& sign_c)
{
	if (sign_a >= 0 && sign_b >= 0)
	{
		int cmpRes = a.CompareTo(b);
		if (cmpRes == 0)
		{
			sign_c = 0;
			c = Integer::ZERO();
		}
		else if (cmpRes > 0)
		{
			c = a.Sub(b);
			sign_c = 1;
		}
		else
		{
			c = b.Sub(a);
			sign_c = -1;
		}
	}
	
	if (sign_a >= 0 && sign_b < 0)
	{
		sign_c = 1;
		c = a.Add(b);
	}

	if (sign_a < 0 && sign_b < 0)
	{
		int cmpRes = a.CompareTo(b);
		if (cmpRes == 0)
		{
			sign_c = 0;
			c = Integer::ZERO();
		}
		else if (cmpRes > 0)
		{
			sign_c = -1;
			c = a.Sub(b);
		}
		else
		{
			sign_c = 1;
			c = b.Sub(a);
		}
	}

	if (sign_a < 0 && sign_b > 0)
	{
		sign_c = -1;
		c = a.Add(b);
	}
}

//求a 关于 b的逆元
//保证a和b互素
Integer Normal_Inv(Integer a, Integer b)
{
	Integer u = Integer::ONE();
	int sign_u = 1;
	Integer e = Integer::ZERO();
	int sign_e = 0;
	Integer v = Integer::ZERO();
	int sign_v = 0;
	Integer f = Integer::ONE();
	int sign_f = 1;

	Integer tmp;
	int sign_tmp;
	Integer mul;

	Integer _b = b;

	while (b.isZero == false)
	{
		Integer q = Divide(a, b);
		Integer r = a.Sub(q.Multiply(b));
		a = b;
		b = r;

		mul = q.Multiply(e);
		Extended_Sub(u, sign_u, mul, sign_e, tmp, sign_tmp);

		u = e;
		sign_u = sign_e;
		e = tmp;
		sign_e = sign_tmp;

		mul = q.Multiply(f);
		Extended_Sub(v, sign_v, mul, sign_f, tmp, sign_tmp);
		v = f;
		sign_v = sign_f;
		f = tmp;
		sign_f = sign_tmp;
	}

	if (sign_u < 0)
	{
		u = _b.Sub(u);
	}

	return u;
}

void GenerateKeys(int bits, Integer &P, Integer &Q, Integer &N, Integer &E, Integer &D)
{
	srand((unsigned)time(NULL));
	MillerInitial();
	P = Montgomery_Generate_Prime(bits>>1);
	Q = Montgomery_Generate_Prime(bits>>1);
	N = P.Multiply(Q);
	Integer Phi = N.Sub(P).Sub(Q);
	Phi.Add1();
	while (true)
	{
		//Phi一定是偶数 这里的E设置为奇数
		E = Integer(bits);
		Integer t = GCD(E, Phi);
		if (t.binLength == 1 && t.A[0] == 1)
		{
			if (E.CompareTo(Phi) == 1)
			{
				E = Mod(E, Phi);
			}
			Integer T;
			D = Normal_Inv(E, Phi);
			break;
		}
	}
}