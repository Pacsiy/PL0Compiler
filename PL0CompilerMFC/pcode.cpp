#include "stdafx.h"
#include "pcode.h"

/*
����һ��Pcode����ָ��
x: ����ָ��
y: Ƕ�ײ���
z: Ѱַ����
*/
void gen(fct x, int y, int z)
{
	if (cx > cxmax) {
		printError(26);
		longjmp(buf, 1);//��������
	}
	if (z > amax) {
		printError(27);
		longjmp(buf, 1);//��������
	}
	code[cx].f = x;
	code[cx].l = y;
	code[cx].a = z;
	cx++;
}

/*
�г�Pcode����ָ��
ii����ʼ��ţ��������
*/
void listcode(int ii)
{
	for (int i = ii; i < cx; i++) {
		//cout << i << " " << mnemonic[code[i].f] << " " << code[i].l << " " << code[i].a << endl;
		const char* str = mnemonic[code[i].f].data();
		fprintf(fcode, "%d %s %d %d\n", i, str, code[i].l, code[i].a);
	}
}