
// PL0CompilerMFCDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PL0CompilerMFC.h"
#include "PL0CompilerMFCDlg.h"
#include "afxdialogex.h"
#include "Init.h"
#include "error.h"
#include "lexica.h"
#include "syntax.h"
#include "sysTab.h"
#include "pcode.h"
#include "interpret.h"
#include "console.h"

char ch;//���¶�ȡ�ַ�
symbol sym;//���¶�ȡ����
string id;//���¶�ȡ��ʶ��
int num; //���¶�ȡ����
int cc;//��ȡ�ַ�����λ��
int ll;//ÿ�д��볤��
int lc;//��ȡ��ǰ�м���
int err;//���������
int cx;//Pcodeָ������ָ��
string line;//����һ�д���

instruction code[cxmax + 5];//Pcode����
string word[norw + 5];//�ؼ���
symbol wsym[norw + 5];//�����ֶ�Ӧ�ķ���ֵ
symbol ssym[256];//����� + �����Ӧ����ֵ

string mnemonic[11];//Pcodeָ���ַ���
symSet declbegsys;//������ʼ�ķ��ż�
symSet statbegsys;//��俪ʼ�ķ��ż�
symSet facbegsys;//���ӿ�ʼ�ķ��ż�
tab table[txmax + 5];//���ű�

ifstream fin;//�ļ���
FILE* fcode;//���Pcode�ļ�
FILE* fallerror;//���������Ϣ

string errmsg[50] = {
	"",
	"����˵����Ӧ��'='������':='",
	"����˵����'='��ӦΪ����",
	"����˵���б�ʶ����ӦΪ'='",
	"const,var,procedure��ӦΪ��ʶ��",
	"©�����Ż�ֺ�",
	"����˵����ķ��Ų���ȷ",
	"ӦΪ��俪ʼ����",
	"����������䲿�ֺ�ķ��Ų���ȷ",
	"�����βӦΪ���",
	"���֮��©�˷ֺ�",
	"��ʶ��δ˵��",
	"������������̸�ֵ",
	"��ֵ�����ӦΪ��ֵ�����':='",
	"call��ӦΪ��ʶ��",
	"call���ʶ������ӦΪ����,���ɵ��ó��������",
	"���������ȱʧthen",
	"ӦΪ�ֺŻ�end",
	"while��ѭ�������ȱʧdo",
	"����ķ��Ų���ȷ",
	"ӦΪ��ϵ�����",
	"���ʽ�ڲ����й��̱�ʶ��",
	"ȱʧ������",
	"���Ӻ󲻿�Ϊ�˷���",
	"���ʽ�����Դ˷��ſ�ʼ"
};

jmp_buf buf;//�Ǳ�����ת

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPL0CompilerMFCDlg �Ի���



CPL0CompilerMFCDlg::CPL0CompilerMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_PL0COMPILERMFC_DIALOG, pParent)
	, inCode(_T(""))
	, fileIN(_T(""))
	, outputInf(_T(""))
	, dataIN(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPL0CompilerMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, inCode);
	DDX_Control(pDX, IDC_LIST2, outPcode);
	DDX_Text(pDX, IDC_MFCEDITBROWSE1, fileIN);
	DDX_Text(pDX, IDC_EDIT4, outputInf);
	DDX_Text(pDX, IDC_EDIT1, dataIN);
}

BEGIN_MESSAGE_MAP(CPL0CompilerMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CPL0CompilerMFCDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CPL0CompilerMFCDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CPL0CompilerMFCDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CPL0CompilerMFCDlg ��Ϣ�������

BOOL CPL0CompilerMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	ShowWindow(SW_MINIMIZE);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	outPcode.SetExtendedStyle(outPcode.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	outPcode.InsertColumn(0, _T("���"), LVCFMT_CENTER, 55, 0);
	outPcode.InsertColumn(1, _T("����"), LVCFMT_CENTER, 80, 0);
	outPcode.InsertColumn(2, _T("����"), LVCFMT_CENTER, 55, 0);
	outPcode.InsertColumn(3, _T("��ַ"), LVCFMT_CENTER, 55, 0);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CPL0CompilerMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPL0CompilerMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CPL0CompilerMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPL0CompilerMFCDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString sentence;
	UpdateData(true);
	GetDlgItemText(IDC_EDIT2, sentence);
	sentence.Replace(_T("\r\n"), _T("\n"));

	/*CFile file;
	file.Open(_T("inCode.txt"), CFile::modeCreate | CFile::modeWrite, NULL);
	file.Write(sentence, sentence.GetLength()*2);
	file.Flush();
	file.Close();*/

	USES_CONVERSION;
	char* s_char = W2A(sentence);
	string inString = s_char;

	if (inString.length() == 0 || inString == "\n" || inString == " ")
	{
		MessageBox(_T("����༭��Ϊ�գ��������������ԣ�"), _T("ERROR"), MB_OK | MB_ICONHAND);
		return;
	}

	ofstream Cout;
	Cout.open("inCode.txt");
	Cout << inString << "\n";
	Cout.close();

	run("inCode.txt");

	//��ʾ������Ϣ
	if (err == 0)
	{
		CEdit* output = (CEdit*)GetDlgItem(IDC_EDIT4);
		output->SetWindowText(_T("����ɹ���\r\n��ܰ��ʾ���������������룬����ǰ������������"));
		UpdateData(true);
	}
	else
	{
		CFile file;
		char * Buf;
		CString code;
		if (file.Open(_T("outError.txt"), CFile::modeRead, NULL))
		{
			int len = file.GetLength();
			Buf = new char[len + 1];
			file.Read(Buf, len);
			Buf[len] = 0;
			code = Buf;
			//code.Replace(_T("\n"), _T("\r\n"));

			CEdit* inputcode = (CEdit*)GetDlgItem(IDC_EDIT4);
			inputcode->SetWindowText(code);
			UpdateData(true);

			delete Buf;
			file.Close();
		}
		else
		{
			MessageBox(_T("��������ļ�ʧ�ܣ������ԣ�"), _T("ERROR"), MB_OK | MB_ICONHAND);
		}
	}

	//��ʾPcode
	outPcode.DeleteAllItems();
	ifstream Cin;
	Cin.open("outPcode.txt");
	string s;
	int i, l, a, ii = -1;
	while (!Cin.eof())
	{
		Cin >> i >> s >> l >> a;
		if (ii == i) break;
		outPcode.InsertItem(i, CString(to_string(i).c_str()));
		outPcode.SetItemText(i, 1, CString(s.c_str()));
		outPcode.SetItemText(i, 2, CString(to_string(l).c_str()));
		outPcode.SetItemText(i, 3, CString(to_string(a).c_str()));
		ii = i;
	}
	Cin.close();
}



void CPL0CompilerMFCDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString selectedPath;
	GetDlgItemText(IDC_MFCEDITBROWSE1, selectedPath);
	USES_CONVERSION;
	char* s_char = W2A(selectedPath);
	string inString = s_char;
	string dir = "";
	for (int i = inString.length() - 1; i >= 0; i--)
	{
		if (inString[i] != '\\') dir = inString[i] + dir;
		else break;
	}

	CFile file;
	char * Buf;
	CString code;
	if (file.Open(CString(dir.c_str()), CFile::modeRead, NULL))
	{
		int len = file.GetLength();
		Buf = new char[len + 1];
		file.Read(Buf, len);
		Buf[len] = 0;
		code = Buf;
		//code.Replace(_T("\n"), _T("\r\n"));

		CEdit* inputcode = (CEdit*)GetDlgItem(IDC_EDIT2);
		inputcode->SetWindowText(code);
		UpdateData(true);

		delete Buf;
		file.Close();
	}
	else
		MessageBox(_T("��������ļ�ʧ�ܣ������ԣ�"), _T("ERROR"), MB_OK | MB_ICONHAND);
}



void CPL0CompilerMFCDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (err == 0)
	{
		CString sentence;
		UpdateData(true);
		GetDlgItemText(IDC_EDIT1, sentence);
		USES_CONVERSION;
		char* s_char = W2A(sentence);
		string inString = s_char;
		ofstream Cout;
		Cout.open("input.txt");
		Cout << inString;
		Cout.close();

		if (interpret() == 1)
		{
			CFile file;
			char * Buf;
			CString code;
			if (file.Open(_T("output.txt"), CFile::modeRead, NULL))
			{
				int len = file.GetLength();
				Buf = new char[len + 1];
				file.Read(Buf, len);
				Buf[len] = 0;
				code = Buf;
				//code.Replace(_T("\n"), _T("\r\n"));

				CEdit* inputcode = (CEdit*)GetDlgItem(IDC_EDIT4);
				inputcode->SetWindowText(code);
				UpdateData(true);

				delete Buf;
				file.Close();
			}
			else
				MessageBox(_T("������ļ�ʧ�ܣ������ԣ�"), _T("ERROR"), MB_OK | MB_ICONHAND);
		}
		else
			MessageBox(_T("�������ݲ��㣬���������룡"), _T("ERROR"), MB_OK | MB_ICONHAND);
	}
	else
		MessageBox(_T("������ڴ����޷����ͳ���"), _T("ERROR"), MB_OK | MB_ICONHAND);
}
