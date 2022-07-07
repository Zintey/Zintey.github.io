/*
	Dictionary
	2022.2.7
	3.5 h + 1.5 h 
*/
#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>

const int MAXN = 3e6 + 10;
using namespace std;

struct DIC {
	string CN, EN;
} Dic[MAXN];


struct Frame {
    COORD position[2];
    int flag;
} Frame;
void SetPos(COORD a)
{
    HANDLE out=GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(out, a);
}
void SetPos(int i, int j)
{
    COORD pos={i, j};
    SetPos(pos);
}
HANDLE hout=GetStdHandle(STD_OUTPUT_HANDLE);
COORD coord;
void hide()
{
    CONSOLE_CURSOR_INFO cursor_info={1,0};
    SetConsoleCursorInfo(hout, &cursor_info);
}
void Color(int color)
{
    /*
    color ==  1 -> blue
              2 -> green
              4 -> red
              7 -> white
    */
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | color);

}

int mark, add, cnt, update;
struct Trie {
	int ch[MAXN][85], sz, val[MAXN];
	Trie ()
	{
		sz = 0;
		memset (ch[0], 0, sizeof (ch[0]));
		memset (val, 0, sizeof (val));
	}
	int char_to_int (char c)
	{
		if ('a' <= c && c <= 'z')
			return c - 'a';
		if ('A' <= c && c <= 'Z');
			return c - 'A' + 26;
		if (c == '_') return 52;
		if (c == ' ') return 53;
		if (c == '-') return 54;
		if (c == '+') return 55;
		if (c == '*') return 56;
		if (c == '/') return 57;
		if (c == '|') return 58;
		if (c == '\\') return 59;
		if (c == '@') return 60;
		if (c == ':') return 61;
		if (c == '?') return 62;
		if (c == '.') return 63;
		if (c == '=') return 64;
		if (c == '%') return 65;
		if (c == '#') return 66;
		if (c == '<') return 67;
		if (c == '>') return 68;
		if (c == '&') return 69;
		if (c == '(') return 70;
		if (c == ')') return 71;
		if (c == '[') return 72;
		if (c == ']') return 73;
		if (c == '~') return 74;
		if ('0' <= c && c <= '9')
			return c - '0' + 75;
	}
	void insert (string s, int id)
	{
		int u = 0, len = s.size ();
		for (int i = 0; i < len; i++)
		{
			int c = char_to_int (s[i]);
			if (!ch[u][c])
			{
				ch[u][c] = ++sz;
				memset (ch[sz], 0, sizeof (ch[sz]));
			}
			u = ch[u][c];
		}
		val[u] = id;
	}
	void findch (int u)
	{
		if (mark >= 10) return;
		for (int i = 0; i < 85; i++)
		{
			if (!ch[u][i]) continue;
			int id = val[ch[u][i]];
			if (id)
			{
				mark++;
				Color (3);
				cout << Dic[id].EN << " <--- " << Dic[id].CN << endl << endl;
				Color (7);
			}
		}
		for (int i = 0; i < 85; i++)
		{
			if (!ch[u][i]) continue;
			findch (ch[u][i]);
		}
	}
	void NOFIND (string s, int u)
	{
		mark = 0;
		findch (u);
		cout << "未找到 ";
		Color (6);
		cout << s << endl;
		Color (7);
		cout << "是否加入词库 (TAB)" << endl;
		add = 1; 
	}
	void search (string s)
	{
		int u = 0, len = s.size ();
		for (int i = 0; i < len; i++)
		{
			int c = char_to_int (s[i]);
			if (!ch[u][c])
				return NOFIND (s, u);
			u = ch[u][c];
			if (val[u])
			{
				Color (8);
				if (i == len - 1) Color (6);
				cout << Dic[val[u]].EN << " <--- " << Dic[val[u]].CN << endl << endl;
				Color (7);
				if (i == len - 1)
				{
					update = val[u];
					mark = 0;
					findch (u); 
					cout << "更改 ";
					Color (6); cout << s; Color (7);
					cout << " 的意思 (TAB)" << endl;
				}
				else
					update = 0;
			}
		}
		if (!val[u])
			return NOFIND (s, u);
	}
} tree;

void build ()
{
	system("chcp 936");
	system ("title Dictionary");
	system("mode con lines=30");
	ifstream in;
	in.open ("Dictionary.DIC");
	cnt = 1;
	int flag = 2;
	system ("cls");
	string s = "词典";
	while (getline (in, s))
	{
		if (flag == 1)
		{
			Dic[cnt].CN = s;
			tree.insert (Dic[cnt].EN, cnt); 
			cnt++;
		}
		if (flag == 2)
		{
			Dic[cnt].EN = s;
		}
		flag = flag == 1 ? 2 : 1;
	}
	in.close ();
}
void save ()
{
	ofstream write;
	write.open ("Dictionary.DIC");
	for (int i = 1; i <= cnt; i++)
		write << Dic[i].EN << endl,
		write << Dic[i].CN << endl;
	write.close ();
}
void work ()
{
//	hide ();
	while (true)
	{
		update = 0, add = 0; 
		system ("cls");
		Color(8);
		cout << "请输入查询的词";
		Color(7); 
		string s = "";
		char c = '\n';
		while (true)
		{
			if(c == '\n' or c == '\r')
				goto FLAG2;
			if (update)
			{
				if(c != 9)
				{
					update = 0;
					goto FLAG;
				}
				cout << s << " 的意思改为：";
				string z;
				getline(cin, z);
				Dic[update].CN = z;
				update = 0;
				save ();
				system ("cls");
				cout << "已更正" << endl;
				system ("pause");
				break;
			}
			if (add && c == 9)
			{
				add = 0;
				cout << s << " 的意思是：";
				string z;
				getline(cin, z);
				Dic[++cnt].EN = s;
				Dic[cnt].CN = z;
				tree.insert (s, cnt);
				save ();
				system ("cls");
				cout << "已添加" << endl;
				system ("pause"); 
				break;
			}
			
			FLAG:
				if (c == 8)
				{
					string tmp = s.substr (0, s.size () - 1);
					s = tmp;
				}
				else
				{
					s = s + char (c);
				}
				if (s == "")
				{
					Color(8);
					cout << "请输入查询的词";
					Color(7); 
				}
			FLAG2:
				Color (6);
				cout << s << endl << endl;
				Color (7);
				if (s == "open dic" || s == "opendic")
				{
					if (system("Dictionary.DIC"))
						system("notepad.exe Dictionary.DIC");
				}
				if (s == "open code" || s == "opencpp" || s == "opencode" || s == "open cpp")
				{
					if (system("Dictionary.cpp"))
						MessageBox(NULL, TEXT("若要查看源代码 请安装.cpp编辑应用 如（DEV-C++）"), TEXT("提示"), MB_OK);
				}
				if (s == "quit dic" || s == "quitdic")
				{
					return;
				}
				tree.search (s);
				SetPos (s.size (), 0);
				c = getch ();
				system ("cls");
		}
	}
}

signed main ()
{
	build ();
	work ();
	return 0;
}
