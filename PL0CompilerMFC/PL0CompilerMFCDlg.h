
// PL0CompilerMFCDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"


// CPL0CompilerMFCDlg �Ի���
class CPL0CompilerMFCDlg : public CDialogEx
{
// ����
public:
	CPL0CompilerMFCDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PL0COMPILERMFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
