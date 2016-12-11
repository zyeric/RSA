typedef unsigned int uint;
typedef unsigned long long ull;

const int BIT = 24;
const int BITMAX = 1<<BIT;
const int MAXLENGTH = 1024/BIT+1;
const int MILLERTIME = 20;

class Integer
{
public:
	//length��ʾA���鱣�����ݵĳ��� ���Ƕ�����λ�ĳ���
	int length;
	//������λ�ĳ���
	int binLength;
	//���ǵ����˷�λ�������� �洢Ϊ�����ռ�
	uint A[MAXLENGTH<<1];
	bool isZero;

	//Ĭ�Ϲ��캯��
	Integer();

	Integer(char *s);
	//�����̶�λ��������
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
	//����d
	Integer SubDecompose();
	//����pС�ڵ�ǰ��
	Integer Sub(Integer p);

	//���ȵ�nλ�ߵ���ȫ��Ϊ0 ������nλ
	//����n > 0
	//ʵ�������� mod 2^n��ֵ
	Integer Remove(int n);

	//���˷�
	Integer Multiply(Integer p);

	//����1��ʾ��ǰ����
	//����0��ʾ���
	//����-1��ʾp��
	int CompareTo(Integer p);

	void PrintDecimal();
	void PrintHex();
};

//N �� bitsλ�� P��Q��bits/2λ������
extern void GenerateKeys(int bits, Integer &P, Integer &Q, Integer &N, Integer &E, Integer &D);
extern Integer Encrypt(Integer X, Integer E, Integer N);
extern Integer Decrypt(Integer X, Integer D, Integer N);