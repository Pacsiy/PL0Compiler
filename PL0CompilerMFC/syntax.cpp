#include "stdafx.h"
#include "syntax.h"

/*
子程序语法分析
lev:当前分程序所在层
tx:符号表当前尾指针
fsys:当前模块后继符号集合
*/
void block(int lev, int tx, symSet fsys)
{
	int dx, tx0, cx0;
	dx = 3;//三个空间存放：静态链SL、动态链DL和返回地址RA
	tx0 = tx;
	table[tx].adr = cx;//记录当前代码层开始位置
	gen(jmp, 0, 0);//产生跳转指令，跳转位置未知暂时填0

	if (lev > levmax)//嵌套层数过多
		printError(32);

	do {
		if (sym == constsym) {
			getsym();
			do {
				constdeclaration(lev, tx, dx);//常量声明处理
				while (sym == comma) {
					getsym();
					constdeclaration(lev, tx, dx);
				}
				if (sym == semicolon)//分号结束
					getsym();
				else//缺少分号
					printError(5);
			} while (sym == ident);
		}
		if (sym == varsym) {
			getsym();
			do {
				vardeclaration(lev, tx, dx);//变量声明处理
				while (sym == comma) {
					getsym();
					vardeclaration(lev, tx, dx);
				}
				if (sym == semicolon)//分号结束
					getsym();
				else//缺少分号
					printError(5);
			} while (sym == ident);
		}

		while (sym == procsym)//处理过程声明
		{
			getsym();
			if (sym == ident) {
				enter(procedure, dx, lev, tx);//添加符号表
				getsym();
			}
			else//procedure后应为标识符
				printError(4);

			if (sym == semicolon)
				getsym();
			else
				printError(5);
			
			symSet tmp;
			tmp.clear();
			tmp.insert(fsys.begin(), fsys.end());
			tmp.insert(semicolon);
			block(lev + 1, tx, tmp);//递归

			if (sym == semicolon) {
				getsym();
				symSet tmp2;
				tmp2.clear();
				tmp2.insert(statbegsys.begin(), statbegsys.end());
				tmp2.insert(ident);
				tmp2.insert(procsym);
				isLegal(tmp2, fsys, 6);
			}
			else
				printError(5);
		}
		symSet tmp3;
		tmp3.clear();
		tmp3.insert(statbegsys.begin(), statbegsys.end());
		tmp3.insert(ident);
		isLegal(tmp3, declbegsys, 7);
	} while (declbegsys.count(sym));

	code[table[tx0].adr].a = cx;
	table[tx0].adr = cx;
	cx0 = cx;
	gen(inta, 0, dx);//添加生成指令，为被调用的过程开辟dx单元的数据区

	symSet tmp;
	tmp.clear();
	tmp.insert(fsys.begin(), fsys.end());
	tmp.insert(semicolon);
	tmp.insert(endsym);
	statement(tmp, lev, tx);//语句处理
	gen(opr, 0, 0);//添加释放指令，过程出口需要释放数据段

	symSet tmp2;
	tmp2.clear();
	isLegal(fsys, tmp2, 8);
}

/*
常量声明处理
*/
void constdeclaration(int lev, int &tx, int &dx)
{
	if (sym == ident)
	{
		getsym();
		if (sym == eql || sym == becomes) {
			if (sym == becomes)//错误处理：应是=而不是:=
				printError(1);
			
			getsym();
			if (sym == number) {
				enter(constant, dx, lev, tx);//填写符号表
				getsym();
			}
			else//错误处理：=后应为数
				printError(2);
		}
		else//错误处理：标识符后应为=
			printError(3);
	}
	else//错误处理：const,var,procedure后应为标识符
		printError(4);
}

/*
变量声明处理
*/
void vardeclaration(int lev, int &tx, int &dx)
{
	if (sym == ident) {
		enter(variable, dx, lev, tx);//填写符号表
		getsym();
	}
	else//错误处理：const,var,procedure后应为标识符
		printError(4);
}

/*
语句处理
*/
void statement(symSet fsys, int lev, int &tx)
{
	int i, cx1, cx2;
	if (sym == ident)
	{
		i = position(id, tx);
		if (i == 0)//错误处理：标识符未说明
			printError(11);
		else if (table[i].kind != variable)//错误处理：不可向常量或过程赋值
		{
			printError(12);
			i = 0;
		}
		getsym();
		if (sym == becomes) {
			getsym();
		}
		else//错误处理：应为赋值运算符:=
			printError(13);

		expression(fsys, lev, tx);
		if (i != 0)
			gen(sto, lev - table[i].level, table[i].adr);//添加赋值指令，将数据栈顶内容存入变量
	}
	else if (sym == callsym)
	{
		getsym();
		if (sym != ident)
			printError(14);
		else
		{
			i = position(id, tx);
			if (i == 0)
				printError(11);
			else
			{
				if (table[i].kind == procedure) {
					gen(cal, lev - table[i].level, table[i].adr);
				}
				else
					printError(15);
			}
			getsym();
		}
	}
	else if (sym == ifsym) {
		getsym();
		symSet tmp;
		tmp.clear();
		tmp.insert(fsys.begin(), fsys.end());
		tmp.insert(thensym);
		tmp.insert(dosym);
		condition(tmp, lev, tx);
		if (sym == thensym)
			getsym();
		else
			printError(16);

		cx1 = cx;
		gen(jpc, 0, 0);
		symSet tmp2;
		tmp2.clear();
		tmp2.insert(fsys.begin(), fsys.end());
		tmp2.insert(elsesym);
		statement(tmp2, lev, tx);

		if (sym == elsesym) {
			getsym();
			cx2 = cx;
			gen(jmp, 0, 0);
			code[cx1].a = cx;
			statement(fsys, lev, tx);
			code[cx2].a = cx;
		}
		else {
			code[cx1].a = cx;
		}
	}
	else if (sym == beginsym) {
		getsym();
		symSet tmp;
		tmp.clear();
		tmp.insert(fsys.begin(), fsys.end());
		tmp.insert(semicolon);
		tmp.insert(endsym);
		statement(tmp, lev, tx);

		symSet tmp2;
		tmp2.clear();
		tmp2.insert(statbegsys.begin(), statbegsys.end());
		tmp2.insert(semicolon);
		while (tmp2.count(sym)) {
			if (sym == semicolon) {
				getsym();
			}
			else//错误处理：语句间漏分号
				printError(10);
			
			statement(tmp, lev, tx);
		}

		if (sym == endsym)
			getsym();
		else//错误处理：缺少end
			printError(17);
	}
	else if (sym == whilesym) {
		cx1 = cx;
		getsym();
		symSet tmp;
		tmp.clear();
		tmp.insert(fsys.begin(), fsys.end());
		tmp.insert(dosym);
		condition(tmp, lev, tx);
		cx2 = cx;
		gen(jpc, 0, 0);
		if (sym == dosym) {
			getsym();
		}
		else {
			printError(18);
		}
		statement(fsys, lev, tx);
		gen(jmp, 0, cx1);
		code[cx2].a = cx;
	}
	else if (sym == readsym) {
		getsym();
		if (sym == lparen) {
			do {
				getsym();
				if (sym == ident) {
					i = position(id, tx);
					if (i == 0) {
						printError(11);
					}
					else {
						if (table[i].kind != variable) {
							printError(12);
							i = 0;
						}
						else {
							gen(red, lev - table[i].level, table[i].adr);
						}
					}
				}
				else {
					printError(28);
				}
				getsym();
			} while (sym == comma);
		}
		else
			printError(40);
		
		if (sym != rparen) {
			printError(22);
		}
		getsym();
	}
	else if (sym == writesym) {
		getsym();
		if (sym == lparen) {
			symSet tmp;
			tmp.clear();
			tmp.insert(fsys.begin(), fsys.end());
			tmp.insert(rparen);
			tmp.insert(comma);
			do {
				getsym();
				expression(tmp, lev, tx);
				gen(wrt, 0, 0);
			} while (sym == comma);

			if (sym != rparen) {
				printError(22);
			}
			getsym();
		}
		else
			printError(40);
	}
	else if (sym == repeatsym) {
		cx1 = cx;
		getsym();
		symSet tmp;
		tmp.clear();
		tmp.insert(fsys.begin(), fsys.end());
		tmp.insert(semicolon);
		tmp.insert(untilsym);
		statement(tmp, lev, tx);

		symSet tmp2;
		tmp2.clear();
		tmp2.insert(beginsym);
		tmp2.insert(callsym);
		tmp2.insert(ifsym);
		tmp2.insert(whilesym);
		tmp2.insert(semicolon);
		while (tmp2.count(sym)) {
			if (sym == semicolon) {
				getsym();
			}
			else {
				printError(5);
			}
			symSet tmp3;
			tmp3.clear();
			tmp3.insert(fsys.begin(), fsys.end());
			tmp3.insert(semicolon);
			tmp3.insert(untilsym);
			statement(tmp3, lev, tx);
		}
		if (sym == untilsym) {
			getsym();
			condition(fsys, lev, tx);
			gen(jpc, 0, cx1);
		}
		else//错误处理：缺少until
			printError(25);
	}
	symSet tmp;
	tmp.clear();
	isLegal(fsys, tmp, 19);//检测语句正确性
}

/*
表达式处理
*/
void expression(symSet fsys, int lev, int &tx)
{
	symbol addop;
	symSet tmp;
	tmp.clear();
	tmp.insert(fsys.begin(), fsys.end());
	tmp.insert(pluss);
	tmp.insert(minuss);

	if (sym == pluss || sym == minuss) {
		addop = sym;
		getsym();
		term(tmp, lev, tx);
		if (addop == minuss) {
			gen(opr, 0, 1);//负号
		}
	}
	else
		term(tmp, lev, tx);

	while (sym == pluss || sym == minuss) {
		addop = sym;
		getsym();
		term(tmp, lev, tx);
		if (addop == pluss)
			gen(opr, 0, 2);//加法指令
		else
			gen(opr, 0, 3);//减法指令
	}
}

/*
项处理
*/
void term(symSet fsys, int lev, int &tx)
{
	symbol mulop;
	symSet tmp;
	tmp.clear();
	tmp.insert(fsys.begin(), fsys.end());
	tmp.insert(times);
	tmp.insert(slash);
	factor(tmp, lev, tx);

	while (sym == times || sym == slash) {
		mulop = sym;
		getsym();
		factor(tmp, lev, tx);
		if (mulop == times)
			gen(opr, 0, 4);//乘法指令
		else
			gen(opr, 0, 5);//除法指令
	}
}

/*
因子处理
*/
void factor(symSet fsys, int lev, int &tx)
{
	int i;
	isLegal(facbegsys, fsys, 24);
	while (facbegsys.count(sym))
	{
		if (sym == ident)//因子为常量or变量
		{
			i = position(id, tx);
			if (i == 0) {
				printError(11);
			}
			else {
				switch (table[i].kind) {
				case constant:
					gen(lit, 0, table[i].val);
					break;
				case variable:
					gen(lod, lev - table[i].level, table[i].adr);
					break;
				case procedure:
					printError(21);//错误处理：表达式内不可有过程标识符
					break;
				default:
					break;
				}
			}
			getsym();
		}
		else if (sym == number)//因子为一个数
		{
			if (num > nmax) {
				printError(30);
				num = 0;
			}
			gen(lit, 0, num);
			getsym();
		}
		else if (sym == lparen)//因子为表达式，递归调用
		{
			getsym();
			symSet tmp;
			tmp.clear();
			tmp.insert(fsys.begin(), fsys.end());
			tmp.insert(rparen);
			expression(tmp, lev, tx);

			if (sym == rparen)
				getsym();
			else
				printError(22);
		}
		symSet tmp;
		tmp.clear();
		tmp.insert(lparen);
		isLegal(fsys, tmp, 23);
	}
}

/*
条件处理
*/
void condition(symSet fsys, int lev, int &tx)
{
	symbol relop;
	if (sym == oddsym) {
		getsym();
		expression(fsys, lev, tx);
		gen(opr, 0, 6);
	}
	else {
		symSet tmp;
		tmp.clear();
		tmp.insert(fsys.begin(), fsys.end());
		tmp.insert(eql);        tmp.insert(neq);        tmp.insert(lss);
		tmp.insert(gtr);        tmp.insert(leq);        tmp.insert(geq);
		expression(tmp, lev, tx);

		tmp.clear();
		tmp.insert(eql);        tmp.insert(neq);        tmp.insert(lss);
		tmp.insert(gtr);        tmp.insert(leq);        tmp.insert(geq);
		if (!tmp.count(sym)) {
			printError(20);//错误处理：应该为关系运算符
		}
		else {
			relop = sym;
			getsym();
			expression(fsys, lev, tx);
			switch (relop)
			{
				case eql:
					gen(opr, 0, 8);
					break;
				case neq:
					gen(opr, 0, 9);
					break;
				case lss:
					gen(opr, 0, 10);
					break;
				case geq:
					gen(opr, 0, 11);
					break;
				case gtr:
					gen(opr, 0, 12);
					break;
				case leq:
					gen(opr, 0, 13);
					break;
				default:
					break;
			}
		}
	}
}