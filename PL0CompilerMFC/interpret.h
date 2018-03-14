#pragma once

//解释Pcode程序

#ifndef __PL0Compiler__interpret__
#define __PL0Compiler__interpret__

#include "Init.h"

const int stacksize = 500;//栈空间大小

int base(int l);//通过过程基址求上一层过程基址
int interpret();//解释程序

#endif