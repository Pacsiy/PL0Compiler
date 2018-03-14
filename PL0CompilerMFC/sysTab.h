#pragma once

//符号表管理函数

#ifndef __PL0Compiler__sysTab__
#define __PL0Compiler__sysTab__

#include "Init.h"
#include "error.h"

void enter(objecttype k, int &dx, int lev, int &tx);//登录符号表，新增符号表项
int position(string id, int tx);//查找标识符在符号表中的位置，从tx开始倒序查找标识符

#endif