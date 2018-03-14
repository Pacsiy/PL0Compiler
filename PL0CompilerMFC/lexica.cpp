#include "stdafx.h"
#include "lexica.h"

void getch()
{
	if (cc == ll)//line��ȡ�գ���ȡ��һ��
	{
		ll = cc = 0;
		getline(fin, line);
		while (!fin.eof() && line == "") {
			getline(fin, line);
		}
		if (line == "" && fin.eof()) {
			cout << "Program incomplete!" << endl;
			longjmp(buf, 1);//��������
		}
		//cout << lc << " ";
		//cout << line << endl;
		ll = line.length();
		line[ll++] = ' ';
		lc++;
	}
	ch = line[cc++];
}

void getsym()
{
	while (ch == ' ' || ch == '\n' || ch == '\t')
		getch();

	if (((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z')))//ʶ��ؼ���or��ʶ��
	{
		int k = 0;
		string a;
		a.clear();
		do {
			if (k < al) {
				a += ch;
				//k++;
			}
			getch();
		} while (((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z')) || ((ch >= '0') && (ch <= '9')));

		k = a.length();
		id.clear();
		id = a;
		int l = 1, r = norw;//���ּ����ؼ��ֱ�
		do {
			k = (l + r) / 2;
			if (id <= word[k])
				r = k - 1;
			if (id >= word[k])
				l = k + 1;
		} while (l <= r);

		if (l - 1 > r)//�ؼ���
			sym = wsym[k];
		else//��ʶ��
			sym = ident;
	}
	else if ((ch >= '0') && (ch <= '9'))//��������
	{
		int k = 0;
		num = 0;
		sym = number;
		do {
			num = 10 * num + (ch - '0');
			k++;
			getch();
		} while ((ch >= '0') && (ch <= '9'));
		if (k > nmax)//����̫��
			printError(30);
	}
	else if (ch == ':')//����:=
	{
		getch();
		if (ch == '=') {
			sym = becomes;
			getch();
		}
		else
			sym = nul;
	}
	else if (ch == '<')//����<��<=��<>
	{
		getch();
		if (ch == '=') {
			sym = leq;
			getch();
		}
		else if (ch == '>') {
			sym = neq;
			getch();
		}
		else
			sym = lss;
	}
	else if (ch == '>')//����>��>=
	{
		getch();
		if (ch == '=') {
			sym = geq;
			getch();
		}
		else
			sym = gtr;
	}
	else//�����ַ�����
	{
		sym = ssym[ch];
		getch();
	}
}

/*
����̷��źϷ���
s1����ǰ�﷨�ɷֺϷ��ĺ�̷���
s2��ֹͣ���ż���
n��������
*/
void isLegal(symSet s1, symSet s2, int n) {
	if (!s1.count(sym)) {
		printError(n);
		//���ִ���ʱ�����϶�ȡ���ʳ��ԡ��޸���
		while (!s1.count(sym) && !s2.count(sym))
			getsym();
	}
}