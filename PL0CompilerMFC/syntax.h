#pragma once

//�﷨��������

#ifndef __PL0Compiler__syntax__
#define __PL0Compiler__syntax__

#include "Init.h"
#include "lexica.h"
#include "sysTab.h"
#include "pcode.h"
#include "error.h"

void block(int lev, int tx, symSet fsys);//�ӳ����﷨����
void constdeclaration(int lev, int &tx, int &dx);//����������
void vardeclaration(int lev, int &tx, int &dx);//�����������

void statement(symSet fsys, int lev, int &tx);//��䴦��
void expression(symSet fsys, int lev, int &tx);//���ʽ����
void term(symSet fsys, int lev, int &tx);//���
void factor(symSet fsys, int lev, int &tx);//���Ӵ���
void condition(symSet fsys, int lev, int &tx);//����������

#endif