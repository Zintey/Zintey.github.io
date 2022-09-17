#include "head.h" 
using namespace std;
int N = 16, M = 44;
int souce, cnt;

void entrance_get_kb()
{
	while(game.over != 1)
	{
		if(_kbhit())
		{
			char c = getch ();
			char k = 'X';
			int col = white;
			switch(c)
			{
				case 'w':
				case 'W':
					if(dir == 2 && len > 1) break;
					dir = 1;
					k = '^';
					break;
				case 's':
				case 'S':
					if(dir == 1 && len > 1) break;
					dir = 2;
					k = 'V';
					break;
				case 'a':
				case 'A':
					if(dir == 4 && len > 1) break;
					dir = 3;
					k = '<'; 
					break;
				case 'd':
				case 'D':
					if(dir == 3 && len > 1) break;
					dir = 4;
					k = '>';
					break;
				case ' ':
					if(game.over < 0) 
						k = '-', game.over = 0;
					else
						k = 'P', game.over = -1;
					col = red;
					break;
			}
			while(_kbhit()) c = getch();
			Print(M / 2, N - 1, k, col);
		}
	}
}

bool check()
{
	int x = head.x, y = head.y;
	if (Map[y][x] > 0)
		return 0;
	return 1;
}

void GAMEOVER()
{
	kColor(4);
	Print(snake[R].x, snake[R].y, 'O');
	game.over = 1;
	Sleep(300);
	system("cls");
	cout << "Gmae Over!!!" << endl;
	cout << "Souce => " << souce << endl;
	cout << "Len => " << len << endl; 
	fstream f;
	f.open("maxsouce.sc");
	int num, maxlen;
	f >> num >> maxlen;
	if (souce > num || (souce == num && len > maxlen))
	{
		cout << "新纪录!!!\n\n\n";
		ofstream ff;
		ff.open("maxsouce.sc");
		ff << souce << " " << len << endl;
		ff.close();
	}
	else
	{
		cout << "历史记录：\n";
		cout << " souce => " << num << endl;
		cout << " len => " << maxlen << "\n\n\n";
	}
	f.close();
	system("pause");
	exit(0);
}

void print()
{
	if(head.x == snake[R].x && head.y == snake[R].y)
		return;
	int x = snake[L].x, y = snake[L].y;
	Print(x, y, '\b');
	Print(x, y, ' ');
	Map[y][x] = 0;
	L++;
	x = head.x, y = head.y;
	Print(x, y, 'O');
	snake[++R] = head;
	if(!check())
		return GAMEOVER();
	if(Map[y][x] < 0)
	{
		souce += -1 * Map[y][x];
		Map[y][x] = 0;
		Print((M - 4) / 2, N, '$', green);
		cnt += randint(1, 4);
		int tmp = souce, pos = 1;
		vector<char> Sou;
		for(; tmp; tmp /= 10)
			Sou.push_back((tmp % 10) + '0');
		reverse(Sou.begin(), Sou.end());
		for(char c : Sou)
			Print((M - 4) / 2 + pos, N, c, white), pos++;
		game.col = blue;
	}
	Map[y][x] = 3;
	if (cnt >= min (len, 2))
	{
		cnt = 0;
		len++;
		L--;
		x = snake[L].x, y = snake[L].y;
		Print(x, y, 'O');
		Map[y][x] = 3;
	}
}

void entrance_main()
{
	head = snake[0];
	while(game.over != 1)
	{
		while(game.over < 0);
		print();
		Sleep(game.speed);
		Snake tmp = head;
		switch(dir)
		{
			case 1:
				head.y = snake[R].y - 1;
				break;
			case 2:
				head.y = snake[R].y + 1;
				break;
			case 3:
				head.x = snake[R].x - 1;
				break;
			case 4:
				head.x = snake[R].x + 1;
				break;
		}
	}
}

void init()
{
	game.col = white; 
	srand(time(NULL));
	system("title Snake");//设置cmd窗口标题
	system("mode con cols=61 lines=30");//窗口宽度高度
	cout << "Speed(0~200ms): ";
	cin >> game.speed;
	system("cls");
	for (int i = 3; i >= 1; i--)
	{
		cout << i;
		Sleep (300);
		cout << '\b';
	}
	system ("cls");
	for (int i = 1; i < N - 1; i++)
		Map[i][0] = Map[i][M - 1] = 2;
	for (int j = 0; j < M; j++)
		Map[0][j] = Map[N - 1][j] = 1;
	for (int i = 0; i < N; i++, printf("\n"))
		for (int j = 0; j < M; j++)
		{
			if (Map[i][j] == 1)
				putchar('-');
			else if (Map[i][j] == 2)
				putchar('|');
			else
				putchar(' ');
		}
}

void entrance_creat_souce()
{
	srand(time(NULL));
	int x = 0, y = 0;
	while(game.over != 1)
	{
		while(game.over < 0); 
		Map[y][x] = 0;
		Print(x, y, '\b');
		Print(x, y, ' ');
		Sleep(game.speed * randint(0, 10));
		while(game.over < 0); 
		x = randint(1, M - 1), y = randint(3, N - 1);
		while(Map[y][x] > 0) x = randint(1, M - 1), y = randint(1, N - 1);
		Map[y][x] = -1 * len * (randint(0, 5) == 0 ? randint(2, 3) : 1) * (200 / game.speed);
		Print(x, y, '$', green);
		Sleep(game.speed * randint(28, 45) * randint(1, 2));
		
	}
}

void entrance_draw()
{
	while(game.over != 1)
	{
		while(!Print_task.empty())
		{
			Pen now = Print_task.front();
			Print_task.pop();
			kColor(now.col); 
			SetPos(now.x, now.y);
			putchar(now.s);
		}
	}
}

void entrance_othre_draw()
{
	while(game.over != 1)
	{
		while(game.col == blue)
		{
			Print(M + 1, (N + 1) / 2, '!', blue);
			while(game.over < 0);
			Sleep(game.speed * (len + 3));
			game.col = white;
			Print(M + 1, (N + 1) / 2, '\b');
			Print(M + 1, (N + 1) / 2, ' ');
		}
	}
}

signed main()
{
	init(); 
	HideCursor();
	thread get_kb(entrance_get_kb);
//	thread doing(entrance_do);
	thread creat_souce(entrance_creat_souce);
	thread Draw(entrance_draw);
	thread Other_Draw(entrance_othre_draw);
	entrance_main();
	get_kb.join();
//	doing.join();
	Draw.join();
	Other_Draw.join(); 
	return 0; 
}
