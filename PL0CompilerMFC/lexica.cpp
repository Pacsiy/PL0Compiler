#include "stdafx.h"
#include "lexica.h"

void getch()
{
	if (cc == ll)//line被取空，读取下一行
	{
		ll = cc = 0;
		getline(fin, line);
		while (!fin.eof() && line == "") {
			getline(fin, line);
		}
		if (line == "" && fin.eof()) {
			cout << "Program incomplete!" << endl;
			longjmp(buf, 1);//跳出程序
		}
		//cout << lc << " ";
		//cout << line << endl;
		ll = line.length();
		line[ll++] = ' ';
		lc++;
	}
	ch = line[cc++];
}

void getsym()
{
	while (ch == ' ' || ch == '\n' || ch == '\t')
		getch();

	if (((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z')))//识别关键字or标识符
	{
		int k = 0;
		string a;
		a.clear();
		do {
			if (k < al) {
				a += ch;
				//k++;
			}
			getch();
		} while (((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z')) || ((ch >= '0') && (ch <= '9')));

		k = a.length();
		id.clear();
		id = a;
		int l = 1, r = norw;//二分检索关键字表
		do {
			k = (l + r) / 2;
			if (id <= word[k])
				r = k - 1;
			if (id >= word[k])
				l = k + 1;
		} while (l <= r);

		if (l - 1 > r)//关键字
			sym = wsym[k];
		else//标识符
			sym = ident;
	}
	else if ((ch >= '0') && (ch <= '9'))//处理数字
	{
		int k = 0;
		num = 0;
		sym = number;
		do {
			num = 10 * num + (ch - '0');
			k++;
			getch();
		} while ((ch >= '0') && (ch <= '9'));
		if (k > nmax)//数字太大
			printError(30);
	}
	else if (ch == ':')//处理:=
	{
		getch();
		if (ch == '=') {
			sym = becomes;
			getch();
		}
		else
			sym = nul;
	}
	else if (ch == '<')//处理<、<=、<>
	{
		getch();
		if (ch == '=') {
			sym = leq;
			getch();
		}
		else if (ch == '>') {
			sym = neq;
			getch();
		}
		else
			sym = lss;
	}
	else if (ch == '>')//处理>、>=
	{
		getch();
		if (ch == '=') {
			sym = geq;
			getch();
		}
		else
			sym = gtr;
	}
	else//按单字符处理
	{
		sym = ssym[ch];
		getch();
	}
}

/*
检测后继符号合法性
s1：当前语法成分合法的后继符号
s2：停止符号集合
n：错误编号
*/
void isLegal(symSet s1, symSet s2, int n) {
	if (!s1.count(sym)) {
		printError(n);
		//出现错误时，不断读取单词尝试“修复”
		while (!s1.count(sym) && !s2.count(sym))
			getsym();
	}
}