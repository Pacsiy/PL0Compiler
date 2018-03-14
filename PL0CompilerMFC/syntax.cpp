#include "stdafx.h"
#include "syntax.h"

/*
�ӳ����﷨����
lev:��ǰ�ֳ������ڲ�
tx:���ű�ǰβָ��
fsys:��ǰģ���̷��ż���
*/
void block(int lev, int tx, symSet fsys)
{
	int dx, tx0, cx0;
	dx = 3;//�����ռ��ţ���̬��SL����̬��DL�ͷ��ص�ַRA
	tx0 = tx;
	table[tx].adr = cx;//��¼��ǰ����㿪ʼλ��
	gen(jmp, 0, 0);//������תָ���תλ��δ֪��ʱ��0

	if (lev > levmax)//Ƕ�ײ�������
		printError(32);

	do {
		if (sym == constsym) {
			getsym();
			do {
				constdeclaration(lev, tx, dx);//������������
				while (sym == comma) {
					getsym();
					constdeclaration(lev, tx, dx);
				}
				if (sym == semicolon)//�ֺŽ���
					getsym();
				else//ȱ�ٷֺ�
					printError(5);
			} while (sym == ident);
		}
		if (sym == varsym) {
			getsym();
			do {
				vardeclaration(lev, tx, dx);//������������
				while (sym == comma) {
					getsym();
					vardeclaration(lev, tx, dx);
				}
				if (sym == semicolon)//�ֺŽ���
					getsym();
				else//ȱ�ٷֺ�
					printError(5);
			} while (sym == ident);
		}

		while (sym == procsym)//�����������
		{
			getsym();
			if (sym == ident) {
				enter(procedure, dx, lev, tx);//��ӷ��ű�
				getsym();
			}
			else//procedure��ӦΪ��ʶ��
				printError(4);

			if (sym == semicolon)
				getsym();
			else
				printError(5);
			
			symSet tmp;
			tmp.clear();
			tmp.insert(fsys.begin(), fsys.end());
			tmp.insert(semicolon);
			block(lev + 1, tx, tmp);//�ݹ�

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
	gen(inta, 0, dx);//�������ָ�Ϊ�����õĹ��̿���dx��Ԫ��������

	symSet tmp;
	tmp.clear();
	tmp.insert(fsys.begin(), fsys.end());
	tmp.insert(semicolon);
	tmp.insert(endsym);
	statement(tmp, lev, tx);//��䴦��
	gen(opr, 0, 0);//����ͷ�ָ����̳�����Ҫ�ͷ����ݶ�

	symSet tmp2;
	tmp2.clear();
	isLegal(fsys, tmp2, 8);
}

/*
������������
*/
void constdeclaration(int lev, int &tx, int &dx)
{
	if (sym == ident)
	{
		getsym();
		if (sym == eql || sym == becomes) {
			if (sym == becomes)//������Ӧ��=������:=
				printError(1);
			
			getsym();
			if (sym == number) {
				enter(constant, dx, lev, tx);//��д���ű�
				getsym();
			}
			else//������=��ӦΪ��
				printError(2);
		}
		else//��������ʶ����ӦΪ=
			printError(3);
	}
	else//������const,var,procedure��ӦΪ��ʶ��
		printError(4);
}

/*
������������
*/
void vardeclaration(int lev, int &tx, int &dx)
{
	if (sym == ident) {
		enter(variable, dx, lev, tx);//��д���ű�
		getsym();
	}
	else//������const,var,procedure��ӦΪ��ʶ��
		printError(4);
}

/*
��䴦��
*/
void statement(symSet fsys, int lev, int &tx)
{
	int i, cx1, cx2;
	if (sym == ident)
	{
		i = position(id, tx);
		if (i == 0)//��������ʶ��δ˵��
			printError(11);
		else if (table[i].kind != variable)//������������������̸�ֵ
		{
			printError(12);
			i = 0;
		}
		getsym();
		if (sym == becomes) {
			getsym();
		}
		else//������ӦΪ��ֵ�����:=
			printError(13);

		expression(fsys, lev, tx);
		if (i != 0)
			gen(sto, lev - table[i].level, table[i].adr);//��Ӹ�ֵָ�������ջ�����ݴ������
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
			else//����������©�ֺ�
				printError(10);
			
			statement(tmp, lev, tx);
		}

		if (sym == endsym)
			getsym();
		else//������ȱ��end
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
		else//������ȱ��until
			printError(25);
	}
	symSet tmp;
	tmp.clear();
	isLegal(fsys, tmp, 19);//��������ȷ��
}

/*
���ʽ����
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
			gen(opr, 0, 1);//����
		}
	}
	else
		term(tmp, lev, tx);

	while (sym == pluss || sym == minuss) {
		addop = sym;
		getsym();
		term(tmp, lev, tx);
		if (addop == pluss)
			gen(opr, 0, 2);//�ӷ�ָ��
		else
			gen(opr, 0, 3);//����ָ��
	}
}

/*
���
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
			gen(opr, 0, 4);//�˷�ָ��
		else
			gen(opr, 0, 5);//����ָ��
	}
}

/*
���Ӵ���
*/
void factor(symSet fsys, int lev, int &tx)
{
	int i;
	isLegal(facbegsys, fsys, 24);
	while (facbegsys.count(sym))
	{
		if (sym == ident)//����Ϊ����or����
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
					printError(21);//���������ʽ�ڲ����й��̱�ʶ��
					break;
				default:
					break;
				}
			}
			getsym();
		}
		else if (sym == number)//����Ϊһ����
		{
			if (num > nmax) {
				printError(30);
				num = 0;
			}
			gen(lit, 0, num);
			getsym();
		}
		else if (sym == lparen)//����Ϊ���ʽ���ݹ����
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
��������
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
			printError(20);//������Ӧ��Ϊ��ϵ�����
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