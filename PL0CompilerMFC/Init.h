#pragma once

//初始化工程全局变量

#ifndef PL0Compiler_Init_h
#define PL0Compiler_Init_h

#include <iostream>
#include <cstdio>
#include <string>
#include <cstring>
#include <fstream>
#include <set>
#include <setjmp.h>
using namespace std;

//编译系统常量定义
const int norw = 16;//关键字个数
const int txmax = 100;//符号表最大长度
const int nmax = INT32_MAX;//最大数字
const int al = 10;//标识符最大长度
const int amax = 2047;//最大地址
const int levmax = 3;//嵌套最大层数
const int cxmax = 200;//Pcode最大指令数量

enum symbol {
	nul, ident, number, pluss, minuss, times, slash, oddsym, eql, neq, lss,
	leq, gtr, geq, lparen, rparen, comma, semicolon, period, becomes,
	beginsym, endsym, ifsym, thensym,whilesym, dosym, callsym, constsym,
	varsym, procsym, readsym, writesym, elsesym, repeatsym, untilsym
};//保留字类型
enum objecttype { 
	constant, variable, procedure
};//符号表类型
enum fct { 
	lit, opr, lod, sto, cal, inta, jmp, jpc, red, wrt
};//P-code操作指令

typedef set<symbol> symSet;//保留字集合

struct instruction {
	fct f;//操作码
	int l;//层次差
	int	a;
};//操作指令
struct tab {
	string name;
	objecttype kind;
	int val, level, adr;
};//符号表项

extern char ch;//最新读取字符
extern symbol sym;//最新读取保留字
extern string id;//最新读取标识符
extern int num;//最新读取数字
extern int cc;//字符所在行计数
extern int ll;//每行代码长度
extern int lc;//读取当前行计数
extern int err;//错误计数器
extern int cx;//Pcode指令索引指针
extern string line;//缓存一行代码

extern instruction code[cxmax + 5];//Pcode代码
extern string word[norw + 5];//关键字
extern symbol wsym[norw + 5];//保留字对应的符号值
extern symbol ssym[256];//运算符+界符对应符号值

extern symSet declbegsys;//声明开始的符号集
extern symSet statbegsys;//语句开始的符号集
extern symSet facbegsys;//因子开始的符号集
extern string mnemonic[11];//Pcode指令字符串
extern tab table[txmax + 5];//符号表
extern string errmsg[50];//错误信息

extern ifstream fin;//输入源码文件
extern FILE* fcode;//输出Pcode文件
extern FILE* fallerror;//输出错误信息

extern jmp_buf buf;//非本地跳转
#endif