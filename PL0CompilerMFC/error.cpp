#include "stdafx.h"
#include "error.h"

void printError(int n)
{
	string str = "Error(" + to_string(lc - 1) + "," + to_string(cc)
		+ "): 错误编号：" + to_string(n)
		+ "，错误信息：" + errmsg[n];
	//cout << "Error(" << lc-1 << ", " << cc << "): 错误代码：" << n << "，错误信息：" << errmsg[n] << endl;
	fprintf(fallerror, "%s\n", str.c_str());
	err++;
}