#pragma once

//�ʷ���������

#ifndef __PL0Compiler__lexica__
#define __PL0Compiler__lexica__

#include "Init.h"
#include "error.h"

void getch();//ȡһ���ַ�
void getsym();//ȡһ������
void isLegal(symSet s1, symSet s2, int n);//���Ե����Ƿ�Ϸ�

#endif
