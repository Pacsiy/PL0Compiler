#include "stdafx.h"
#include "pcode.h"

/*
新增一条Pcode操作指令
x: 操作指令
y: 嵌套层数
z: 寻址变量
*/
void gen(fct x, int y, int z)
{
	if (cx > cxmax) {
		printError(26);
		longjmp(buf, 1);//跳出程序
	}
	if (z > amax) {
		printError(27);
		longjmp(buf, 1);//跳出程序
	}
	code[cx].f = x;
	code[cx].l = y;
	code[cx].a = z;
	cx++;
}

/*
列出Pcode操作指令
ii：起始编号，方便调试
*/
void listcode(int ii)
{
	for (int i = ii; i < cx; i++) {
		//cout << i << " " << mnemonic[code[i].f] << " " << code[i].l << " " << code[i].a << endl;
		const char* str = mnemonic[code[i].f].data();
		fprintf(fcode, "%d %s %d %d\n", i, str, code[i].l, code[i].a);
	}
}