#include <bits/stdc++.h>
#include <windows.h>
#include <conio.h>

#define blue 1
#define green 2
#define red 4
#define white 7
using namespace std;

const int MAXN = 5e6;
//const mx[] = {0, 0, -1, 1};
//const my[] = {1, -1, 0, 0};
struct Snake {
	int x, y;
} snake[MAXN] = {2, 2}, head;
int L = 0, R = 0, len = 1, dir = 4;

int Map[200][200];

struct Game{
	int speed, col, over;
	Game(int a = 1, int b = 0, int c = white) : speed(a), over(b), col(c) {}
} game;


struct Pen {
	int x, y, col;
	char s;
	Pen(int X, int Y, char S, int C) : x(X), y(Y), s(S), col(C){}
};
queue<Pen> Print_task; 

void Print(int x, int y, char s, int col = game.col)
{
	Print_task.push(Pen(x, y, s, col));
}

typedef struct Frame
{
    COORD position[2];
    int flag;
}Frame;

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

void SetPos(int i, int j, char c)
{
    COORD pos={i, j};
    SetPos(pos);
	putchar(c);
}

void kColor(int color)
{
    /*
    color ==  1 -> blue
              2 -> green
              4 -> red
              7 -> white
    */
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | color);

}

void HideCursor() // ?于隐藏光标
{
	CONSOLE_CURSOR_INFO cursor_info = {1, 0};  // 第?个值为0表?隐藏光标 
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
//函数和结构体都在windows.h中定义。
}

int randint(int l, int r)
{
	return (rand() % (r - l + 1)) + l;
}
