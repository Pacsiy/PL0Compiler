#include "stdafx.h"
#include "sysTab.h"

/*
��¼���ű��������ű���
k:��ʶ�������࣬����Ϊconst��var��procedure
dx:��ǰӦ����ı�������Ե�ַ�������Ҫ����1
lev:��ʶ�����ڵĲ��
tx:���ű�βָ���ָ�룬����ֱ�Ӹı���ű�βָ���ֵ
*/
void enter(objecttype k, int &dx, int lev, int &tx)
{
	table[++tx].name = id;
	table[tx].kind = k;
	switch (k)
	{
		case constant:
			if (num > nmax)//�����������̫��
			{
				printError(30);
				num = 0;
			}
			table[tx].val = num;
			break;
		case variable:
			table[tx].level = lev;
			table[tx].adr = dx++;
			break;
		case procedure:
			table[tx].level = lev;
			break;
		default:
			break;
	}
}

/*
���ұ�ʶ���ڷ��ű��е�λ�ã���tx��ʼ������ұ�ʶ�����ҵ��򷵻��ڷ��ű��е�λ�ã����򷵻�0��
id:Ҫ���ҵı�ʶ������
tx:��ǰ���ű�βָ�루������ң�
*/
int position(string id, int tx)
{
	table[0].name = id;
	int i = tx;
	while (table[i].name != id)
		i--;
	return i;
}