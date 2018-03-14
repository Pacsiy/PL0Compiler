#include "stdafx.h"
#include "interpret.h"

int p, bp, top;
int S[stacksize+1] = { 0 };//ģ��ջʽ�����
instruction ins;//ָ��Ĵ���

/*
ͨ�����̻�ַ����һ����̻�ַ
*/
int base(int l)
{
	int bb = bp;
	while (l > 0) {
		bb = S[bb];
		l--;
	}
	return bb;
}

/*
���ͳ���
*/
int interpret()
{
	//cout << "**********��ʼ���ͳ���**********" << endl;
	ifstream Cin;
	Cin.open("input.txt");
	ofstream Cout;
	Cout.open("output.txt");
	
	top = p = 0;
	bp = 1;
	S[1] = S[2] = S[3] = 0;

	do {
		ins = code[p];
		++p;
		switch (ins.f)
		{
			case lit://ȡ��������ջ��
				top++;
				S[top] = ins.a;
				break;
			case opr://����
				switch (ins.a)
				{
					case 0://�������÷���
						top = bp - 1;
						p = S[top + 3];
						bp = S[top + 2];
						break;
					case 1://����
						S[top] = -S[top];
						break;
					case 2://��
						top--;
						S[top] += S[top + 1];
						break;
					case 3://��
						top--;
						S[top] -= S[top + 1];
						break;
					case 4://��
						top--;
						S[top] *= S[top + 1];
						break;
					case 5://��
						top--;
						S[top] /= S[top + 1];
						break;
					case 6://��ż�ж�
						S[top] %= 2;
						break;
					case 8://�ж����
						top--;
						S[top] = (S[top] == S[top + 1]);
						break;
					case 9://�жϲ���
						top--;
						S[top] = (S[top] != S[top + 1]);
						break;
					case 10://�ж�С��
						top--;
						S[top] = (S[top] < S[top + 1]);
						break;
					case 11://�жϴ��ڵ���
						top--;
						S[top] = (S[top] >= S[top + 1]);
						break;
					case 12://�жϴ���
						top--;
						S[top] = (S[top] > S[top + 1]);
						break;
					case 13://�ж�С�ڵ���
						top--;
						S[top] = (S[top] <= S[top + 1]);
						break;
					default:
						break;
				}
				break;
			case lod://ȡ����ֵ����ջ��
				top++;
				S[top] = S[base(ins.l) + ins.a];
				break;
			case sto://ջ��ֵ���ڱ���
				S[base(ins.l) + ins.a] = S[top];
				top--;
				break;
			case cal://���ù���
				S[top + 1] = base(ins.l);
				S[top + 2] = bp;
				S[top + 3] = p;
				bp = top + 1;
				p = ins.a;
				break;
			case inta://����ռ䣬ָ��+a
				top += ins.a;
				break;
			case jmp://��������ת��a
				p = ins.a;
				break;
			case jpc://������ת��a
				if (S[top] == 0)
					p = ins.a;
				top--;
				break;
			case red://������
				if (Cin.eof())
					return 0;
				Cin >> S[base(ins.l) + ins.a];
				break;
			case wrt://д����
				Cout << S[top] << endl;
				top++;
				break;
			default:
				break;
		}
	} while (p != 0);

	Cin.close();
	Cout.close();
	//cout << "���ͳ����������ӭ�´�ʹ��" << endl;
	return 1;
}