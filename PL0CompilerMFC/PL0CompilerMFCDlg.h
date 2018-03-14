
// PL0CompilerMFCDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"


// CPL0CompilerMFCDlg 对话框
class CPL0CompilerMFCDlg : public CDialogEx
{
// 构造
public:
	CPL0CompilerMFCDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PL0COMPILERMFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString inCode;
	afx_msg void OnBnClickedButton1();
	CListCtrl outPcode;
	afx_msg void OnBnClickedButton3();
	CString fileIN;
	CString outputInf;
	afx_msg void OnBnClickedButton2();
	CString dataIN;
};
