#include "stdafx.h"
#include "error.h"

void printError(int n)
{
	string str = "Error(" + to_string(lc - 1) + "," + to_string(cc)
		+ "): �����ţ�" + to_string(n)
		+ "��������Ϣ��" + errmsg[n];
	//cout << "Error(" << lc-1 << ", " << cc << "): ������룺" << n << "��������Ϣ��" << errmsg[n] << endl;
	fprintf(fallerror, "%s\n", str.c_str());
	err++;
}