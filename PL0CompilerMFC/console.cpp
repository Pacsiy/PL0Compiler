// PL0CompilerWin32.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "console.h"

void init()
{
	err = cc = cx = ll = lc = 0;
	ch = ' ';

	errmsg[25] = "repeat循环语句中缺失until";
	errmsg[26] = "代码太长，无法编译";
	errmsg[27] = "RuntimeError，地址偏移越界";
	errmsg[28] = "Read语句括号内不是标识符";
	errmsg[30] = "这个数太大，超过INT32_MAX";
	errmsg[40] = "缺失左括号";

	//初始化关键字
	word[1] = "begin";		word[2] = "call";		word[3] = "const";		word[4] = "do";
	word[5] = "else";		word[6] = "end";		word[7] = "if";			word[8] = "odd";
	word[9] = "procedure";	word[10] = "read";		word[11] = "repeat";	word[12] = "then";
	word[13] = "until";		word[14] = "var";		word[15] = "while";		word[16] = "write";
	//关键字符号
	wsym[1] = beginsym;     wsym[2] = callsym;      wsym[3] = constsym;     wsym[4] = dosym;
	wsym[5] = elsesym;      wsym[6] = endsym;       wsym[7] = ifsym;        wsym[8] = oddsym;
	wsym[9] = procsym;      wsym[10] = readsym;     wsym[11] = repeatsym;	wsym[12] = thensym;
	wsym[13] = untilsym;    wsym[14] = varsym;      wsym[15] = whilesym;    wsym[16] = writesym;
	//运算符&界符符号
	memset(ssym, nul, sizeof(ssym));
	ssym['+'] = pluss;      ssym['-'] = minuss;     ssym['*'] = times;      ssym['/'] = slash;
	ssym['('] = lparen;     ssym[')'] = rparen;     ssym['='] = eql;        ssym[','] = comma;
	ssym['.'] = period;     ssym['<'] = lss;        ssym['>'] = gtr;        ssym[';'] = semicolon;

	mnemonic[lit] = "LIT"; mnemonic[opr] = "OPR"; mnemonic[lod] = "LOD"; mnemonic[sto] = "STO";
	mnemonic[cal] = "CAL"; mnemonic[inta] = "INT"; mnemonic[jmp] = "JMP"; mnemonic[jpc] = "JPC";
	mnemonic[red] = "RED"; mnemonic[wrt] = "WRT";
	//声明开始的符号集初始化
	declbegsys.clear();
	declbegsys.insert(constsym);
	declbegsys.insert(varsym);
	declbegsys.insert(procsym);
	//语句开始的符号集初始化
	statbegsys.clear();
	statbegsys.insert(beginsym);
	statbegsys.insert(callsym);
	statbegsys.insert(ifsym);
	statbegsys.insert(whilesym);
	statbegsys.insert(repeatsym);
	//因子开始的符号集初始化
	facbegsys.clear();
	facbegsys.insert(ident);
	facbegsys.insert(number);
	facbegsys.insert(lparen);
}

int run(string path)
{
	//cout << "PL/0编译系统启动" << endl;

	fin.open(path.c_str());
	//fin.open("1.txt");
	fallerror = fopen("outError.txt", "w");

	init();//初始化
	getsym();//取得第一个单词
	
	symSet tmp;
	tmp.clear();
	tmp.insert(declbegsys.begin(), declbegsys.end());
	tmp.insert(statbegsys.begin(), statbegsys.end());
	tmp.insert(period);

	if (!setjmp(buf)) {
		block(0, 0, tmp);
	}

	if (sym != period) {
		printError(9);
	}

	if (err > 0)
	{
		string str = "代码段中共有 " + to_string(err) + " 个错误!";
		fprintf(fallerror, "%s", str.c_str());
	}
	fclose(fallerror);

	fcode = fopen("outPcode.txt", "w");
	listcode(0);

	fclose(fcode);
	fin.close();

	//cout << "PL/0编译系统卸载" << endl;
	return 0;
}