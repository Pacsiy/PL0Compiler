// PL0CompilerWin32.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "console.h"

void init()
{
	err = cc = cx = ll = lc = 0;
	ch = ' ';

	errmsg[25] = "repeatѭ�������ȱʧuntil";
	errmsg[26] = "����̫�����޷�����";
	errmsg[27] = "RuntimeError����ַƫ��Խ��";
	errmsg[28] = "Read��������ڲ��Ǳ�ʶ��";
	errmsg[30] = "�����̫�󣬳���INT32_MAX";
	errmsg[40] = "ȱʧ������";

	//��ʼ���ؼ���
	word[1] = "begin";		word[2] = "call";		word[3] = "const";		word[4] = "do";
	word[5] = "else";		word[6] = "end";		word[7] = "if";			word[8] = "odd";
	word[9] = "procedure";	word[10] = "read";		word[11] = "repeat";	word[12] = "then";
	word[13] = "until";		word[14] = "var";		word[15] = "while";		word[16] = "write";
	//�ؼ��ַ���
	wsym[1] = beginsym;     wsym[2] = callsym;      wsym[3] = constsym;     wsym[4] = dosym;
	wsym[5] = elsesym;      wsym[6] = endsym;       wsym[7] = ifsym;        wsym[8] = oddsym;
	wsym[9] = procsym;      wsym[10] = readsym;     wsym[11] = repeatsym;	wsym[12] = thensym;
	wsym[13] = untilsym;    wsym[14] = varsym;      wsym[15] = whilesym;    wsym[16] = writesym;
	//�����&�������
	memset(ssym, nul, sizeof(ssym));
	ssym['+'] = pluss;      ssym['-'] = minuss;     ssym['*'] = times;      ssym['/'] = slash;
	ssym['('] = lparen;     ssym[')'] = rparen;     ssym['='] = eql;        ssym[','] = comma;
	ssym['.'] = period;     ssym['<'] = lss;        ssym['>'] = gtr;        ssym[';'] = semicolon;

	mnemonic[lit] = "LIT"; mnemonic[opr] = "OPR"; mnemonic[lod] = "LOD"; mnemonic[sto] = "STO";
	mnemonic[cal] = "CAL"; mnemonic[inta] = "INT"; mnemonic[jmp] = "JMP"; mnemonic[jpc] = "JPC";
	mnemonic[red] = "RED"; mnemonic[wrt] = "WRT";
	//������ʼ�ķ��ż���ʼ��
	declbegsys.clear();
	declbegsys.insert(constsym);
	declbegsys.insert(varsym);
	declbegsys.insert(procsym);
	//��俪ʼ�ķ��ż���ʼ��
	statbegsys.clear();
	statbegsys.insert(beginsym);
	statbegsys.insert(callsym);
	statbegsys.insert(ifsym);
	statbegsys.insert(whilesym);
	statbegsys.insert(repeatsym);
	//���ӿ�ʼ�ķ��ż���ʼ��
	facbegsys.clear();
	facbegsys.insert(ident);
	facbegsys.insert(number);
	facbegsys.insert(lparen);
}

int run(string path)
{
	//cout << "PL/0����ϵͳ����" << endl;

	fin.open(path.c_str());
	//fin.open("1.txt");
	fallerror = fopen("outError.txt", "w");

	init();//��ʼ��
	getsym();//ȡ�õ�һ������
	
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
		string str = "������й��� " + to_string(err) + " ������!";
		fprintf(fallerror, "%s", str.c_str());
	}
	fclose(fallerror);

	fcode = fopen("outPcode.txt", "w");
	listcode(0);

	fclose(fcode);
	fin.close();

	//cout << "PL/0����ϵͳж��" << endl;
	return 0;
}