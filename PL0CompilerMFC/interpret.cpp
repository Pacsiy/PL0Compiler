#include "stdafx.h"
#include "interpret.h"

int p, bp, top;
int S[stacksize+1] = { 0 };//模拟栈式计算机
instruction ins;//指令寄存器

/*
通过过程基址求上一层过程基址
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
解释程序
*/
int interpret()
{
	//cout << "**********开始解释程序**********" << endl;
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
			case lit://取常量置于栈顶
				top++;
				S[top] = ins.a;
				break;
			case opr://计算
				switch (ins.a)
				{
					case 0://函数调用返回
						top = bp - 1;
						p = S[top + 3];
						bp = S[top + 2];
						break;
					case 1://负号
						S[top] = -S[top];
						break;
					case 2://加
						top--;
						S[top] += S[top + 1];
						break;
					case 3://减
						top--;
						S[top] -= S[top + 1];
						break;
					case 4://乘
						top--;
						S[top] *= S[top + 1];
						break;
					case 5://除
						top--;
						S[top] /= S[top + 1];
						break;
					case 6://奇偶判断
						S[top] %= 2;
						break;
					case 8://判断相等
						top--;
						S[top] = (S[top] == S[top + 1]);
						break;
					case 9://判断不等
						top--;
						S[top] = (S[top] != S[top + 1]);
						break;
					case 10://判断小于
						top--;
						S[top] = (S[top] < S[top + 1]);
						break;
					case 11://判断大于等于
						top--;
						S[top] = (S[top] >= S[top + 1]);
						break;
					case 12://判断大于
						top--;
						S[top] = (S[top] > S[top + 1]);
						break;
					case 13://判断小于等于
						top--;
						S[top] = (S[top] <= S[top + 1]);
						break;
					default:
						break;
				}
				break;
			case lod://取变量值置于栈顶
				top++;
				S[top] = S[base(ins.l) + ins.a];
				break;
			case sto://栈顶值存于变量
				S[base(ins.l) + ins.a] = S[top];
				top--;
				break;
			case cal://调用过程
				S[top + 1] = base(ins.l);
				S[top + 2] = bp;
				S[top + 3] = p;
				bp = top + 1;
				p = ins.a;
				break;
			case inta://分配空间，指针+a
				top += ins.a;
				break;
			case jmp://无条件跳转至a
				p = ins.a;
				break;
			case jpc://条件跳转至a
				if (S[top] == 0)
					p = ins.a;
				top--;
				break;
			case red://读数据
				if (Cin.eof())
					return 0;
				Cin >> S[base(ins.l) + ins.a];
				break;
			case wrt://写数据
				Cout << S[top] << endl;
				top++;
				break;
			default:
				break;
		}
	} while (p != 0);

	Cin.close();
	Cout.close();
	//cout << "解释程序结束，欢迎下次使用" << endl;
	return 1;
}