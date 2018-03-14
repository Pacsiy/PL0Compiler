#pragma once

//程序处理入口

#ifndef __PL0Compiler__console__
#define __PL0Compiler__console__

#include "stdafx.h"
#include "Init.h"
#include "error.h"
#include "lexica.h"
#include "syntax.h"
#include "sysTab.h"
#include "pcode.h"
#include "interpret.h"

void init();
int run(string path);

#endif