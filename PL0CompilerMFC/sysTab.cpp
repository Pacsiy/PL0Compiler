#include "stdafx.h"
#include "sysTab.h"

/*
登录符号表，新增符号表项
k:标识符的种类，可以为const，var或procedure
dx:当前应分配的变量的相对地址，分配后要增加1
lev:标识符所在的层次
tx:符号表尾指针的指针，可以直接改变符号表尾指针的值
*/
void enter(objecttype k, int &dx, int lev, int &tx)
{
	table[++tx].name = id;
	table[tx].kind = k;
	switch (k)
	{
		case constant:
			if (num > nmax)//错误处理：这个数太大
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
查找标识符在符号表中的位置，从tx开始倒序查找标识符。找到则返回在符号表中的位置，否则返回0。
id:要查找的标识符名字
tx:当前符号表尾指针（倒序查找）
*/
int position(string id, int tx)
{
	table[0].name = id;
	int i = tx;
	while (table[i].name != id)
		i--;
	return i;
}