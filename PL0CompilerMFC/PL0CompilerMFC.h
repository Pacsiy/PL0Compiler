
// PL0CompilerMFC.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CPL0CompilerMFCApp: 
// �йش����ʵ�֣������ PL0CompilerMFC.cpp
//

class CPL0CompilerMFCApp : public CWinApp
{
public:
	CPL0CompilerMFCApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CPL0CompilerMFCApp theApp;