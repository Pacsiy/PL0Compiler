#pragma once

//Pcode处理函数

#ifndef __PL0Compiler__pcode__
#define __PL0Compiler__pcode__

#include "Init.h"
#include "error.h"

void gen(fct x, int y, int z);//新增一条Pcode操作指令
void listcode(int ii);//列出Pcode操作指令

#endif