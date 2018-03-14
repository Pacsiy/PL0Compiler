
// PL0CompilerMFCDlg.cpp : 实现文件
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

char ch;//最新读取字符
symbol sym;//最新读取类型
string id;//最新读取标识符
int num; //最新读取数字
int cc;//读取字符所在位置
int ll;//每行代码长度
int lc;//读取当前行计数
int err;//错误计数器
int cx;//Pcode指令索引指针
string line;//缓存一行代码

instruction code[cxmax + 5];//Pcode代码
string word[norw + 5];//关键字
symbol wsym[norw + 5];//保留字对应的符号值
symbol ssym[256];//运算符 + 界符对应符号值

string mnemonic[11];//Pcode指令字符串
symSet declbegsys;//声明开始的符号集
symSet statbegsys;//语句开始的符号集
symSet facbegsys;//因子开始的符号集
tab table[txmax + 5];//符号表

ifstream fin;//文件流
FILE* fcode;//输出Pcode文件
FILE* fallerror;//输出错误信息

string errmsg[50] = {
	"",
	"常数说明中应是'='而不是':='",
	"常数说明中'='后应为数字",
	"常数说明中标识符后应为'='",
	"const,var,procedure后应为标识符",
	"漏掉逗号或分号",
	"过程说明后的符号不正确",
	"应为语句开始符号",
	"程序体内语句部分后的符号不正确",
	"程序结尾应为句号",
	"语句之间漏了分号",
	"标识符未说明",
	"不可向常量或过程赋值",
	"赋值语句中应为赋值运算符':='",
	"call后应为标识符",
	"call后标识符属性应为过程,不可调用常量或变量",
	"条件语句中缺失then",
	"应为分号或end",
	"while型循环语句中缺失do",
	"语句后的符号不正确",
	"应为关系运算符",
	"表达式内不可有过程标识符",
	"缺失右括号",
	"因子后不可为此符号",
	"表达式不能以此符号开始"
};

jmp_buf buf;//非本地跳转

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CPL0CompilerMFCDlg 对话框



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


// CPL0CompilerMFCDlg 消息处理程序

BOOL CPL0CompilerMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MINIMIZE);

	// TODO: 在此添加额外的初始化代码
	outPcode.SetExtendedStyle(outPcode.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	outPcode.InsertColumn(0, _T("序号"), LVCFMT_CENTER, 55, 0);
	outPcode.InsertColumn(1, _T("操作"), LVCFMT_CENTER, 80, 0);
	outPcode.InsertColumn(2, _T("层数"), LVCFMT_CENTER, 55, 0);
	outPcode.InsertColumn(3, _T("地址"), LVCFMT_CENTER, 55, 0);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CPL0CompilerMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CPL0CompilerMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPL0CompilerMFCDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
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
		MessageBox(_T("代码编辑区为空，请输入代码后重试！"), _T("ERROR"), MB_OK | MB_ICONHAND);
		return;
	}

	ofstream Cout;
	Cout.open("inCode.txt");
	Cout << inString << "\n";
	Cout.close();

	run("inCode.txt");

	//显示错误信息
	if (err == 0)
	{
		CEdit* output = (CEdit*)GetDlgItem(IDC_EDIT4);
		output->SetWindowText(_T("编译成功！\r\n温馨提示：程序中若有输入，解释前请先输入数据"));
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
			MessageBox(_T("导入错误文件失败，请重试！"), _T("ERROR"), MB_OK | MB_ICONHAND);
		}
	}

	//显示Pcode
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
	// TODO: 在此添加控件通知处理程序代码
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
		MessageBox(_T("导入代码文件失败，请重试！"), _T("ERROR"), MB_OK | MB_ICONHAND);
}



void CPL0CompilerMFCDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
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
				MessageBox(_T("打开输出文件失败，请重试！"), _T("ERROR"), MB_OK | MB_ICONHAND);
		}
		else
			MessageBox(_T("输入数据不足，请重新输入！"), _T("ERROR"), MB_OK | MB_ICONHAND);
	}
	else
		MessageBox(_T("代码存在错误，无法解释程序！"), _T("ERROR"), MB_OK | MB_ICONHAND);
}
