#pragma once

//词法分析函数

#ifndef __PL0Compiler__lexica__
#define __PL0Compiler__lexica__

#include "Init.h"
#include "error.h"

void getch();//取一个字符
void getsym();//取一个单词
void isLegal(symSet s1, symSet s2, int n);//测试单词是否合法

#endif
