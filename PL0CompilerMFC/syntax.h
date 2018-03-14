#pragma once

//语法分析函数

#ifndef __PL0Compiler__syntax__
#define __PL0Compiler__syntax__

#include "Init.h"
#include "lexica.h"
#include "sysTab.h"
#include "pcode.h"
#include "error.h"

void block(int lev, int tx, symSet fsys);//子程序语法分析
void constdeclaration(int lev, int &tx, int &dx);//处理常量声明
void vardeclaration(int lev, int &tx, int &dx);//处理变量声明

void statement(symSet fsys, int lev, int &tx);//语句处理
void expression(symSet fsys, int lev, int &tx);//表达式处理
void term(symSet fsys, int lev, int &tx);//项处理
void factor(symSet fsys, int lev, int &tx);//因子处理
void condition(symSet fsys, int lev, int &tx);//条件语句分析

#endif