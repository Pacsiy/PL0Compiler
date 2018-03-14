#pragma once

//��ʼ������ȫ�ֱ���

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

//����ϵͳ��������
const int norw = 16;//�ؼ��ָ���
const int txmax = 100;//���ű���󳤶�
const int nmax = INT32_MAX;//�������
const int al = 10;//��ʶ����󳤶�
const int amax = 2047;//����ַ
const int levmax = 3;//Ƕ��������
const int cxmax = 200;//Pcode���ָ������

enum symbol {
	nul, ident, number, pluss, minuss, times, slash, oddsym, eql, neq, lss,
	leq, gtr, geq, lparen, rparen, comma, semicolon, period, becomes,
	beginsym, endsym, ifsym, thensym,whilesym, dosym, callsym, constsym,
	varsym, procsym, readsym, writesym, elsesym, repeatsym, untilsym
};//����������
enum objecttype { 
	constant, variable, procedure
};//���ű�����
enum fct { 
	lit, opr, lod, sto, cal, inta, jmp, jpc, red, wrt
};//P-code����ָ��

typedef set<symbol> symSet;//�����ּ���

struct instruction {
	fct f;//������
	int l;//��β�
	int	a;
};//����ָ��
struct tab {
	string name;
	objecttype kind;
	int val, level, adr;
};//���ű���

extern char ch;//���¶�ȡ�ַ�
extern symbol sym;//���¶�ȡ������
extern string id;//���¶�ȡ��ʶ��
extern int num;//���¶�ȡ����
extern int cc;//�ַ������м���
extern int ll;//ÿ�д��볤��
extern int lc;//��ȡ��ǰ�м���
extern int err;//���������
extern int cx;//Pcodeָ������ָ��
extern string line;//����һ�д���

extern instruction code[cxmax + 5];//Pcode����
extern string word[norw + 5];//�ؼ���
extern symbol wsym[norw + 5];//�����ֶ�Ӧ�ķ���ֵ
extern symbol ssym[256];//�����+�����Ӧ����ֵ

extern symSet declbegsys;//������ʼ�ķ��ż�
extern symSet statbegsys;//��俪ʼ�ķ��ż�
extern symSet facbegsys;//���ӿ�ʼ�ķ��ż�
extern string mnemonic[11];//Pcodeָ���ַ���
extern tab table[txmax + 5];//���ű�
extern string errmsg[50];//������Ϣ

extern ifstream fin;//����Դ���ļ�
extern FILE* fcode;//���Pcode�ļ�
extern FILE* fallerror;//���������Ϣ

extern jmp_buf buf;//�Ǳ�����ת
#endif