// 22.6.19 
#include "head.h" 
using namespace std;
int N = 20, M = 20;
int souce, cnt, add;
void entrance_get_kb()
{
	while(game.over != 1)
	{
		if(_kbhit())
		{
			char c = getch (), ex = '0';
			if(_kbhit()) ex = getch();
			if(ex != '0')
			{
				switch(ex)
				{
					case 'H':
						c = 'W';
						break;
					case 'P':
						c = 'S';
						break;
					case 'K':
						c = 'A';
						break;
					case 'M':
						c = 'D';
						break;
				}
			}
			string k = "×";
			int col = white;
			switch(c)
			{
				case 'w':
				case 'W':
					if(nowdir == 2 && len > 1) break;
					dir = 1;
					k = "↑";
					break;
				case 's':
				case 'S':
					if(nowdir == 1 && len > 1) break;
					dir = 2;
					k = "↓";
					break;
				case 'a':
				case 'A':
					if(nowdir == 4 && len > 1) break;
					dir = 3;
					k = "←"; 
					break;
				case 'd':
				case 'D':
					if(nowdir == 3 && len > 1) break;
					dir = 4;
					k = "→";
					break;
//				case ' ':
//					if(game.over < 0) 
//						k = "‖", game.over = 0;
//					else
//						k = "◎", game.over = -1;
//					col = red;
//					break;
			}
			while(_kbhit()) c = getch();
			Print(M / 2, N, k, col);
		}
	}
}

bool check()
{
	if (head.x < 0) head.x = M - 1;
	if (head.x >= M) head.x = 0;
	if (head.y < 0) head.y = N - 1;
	if (head.y >= N) head.y = 0; 
	int x = head.x, y = head.y;
	fstream log("log.txt", ios::app);
		log << "<" << x << ", " << y << ", " << Map[y][x]; 
	if (Map[y][x] > 0)
		return 0;
	log << ">\n";
	return 1;
}

void GAMEOVER()
{
	Print(snake[R].x, snake[R].y, "■", red);
	Sleep(300);
	Print(M, (N + 1) / 2, " Game Over!!!", red);
	Print(M, (N + 1) / 2 + 1, "按任意键继续", red);
	game.over = 1;
	char c = getch();
	kColor(white);
	system("cls");
	cout << "Gmae Over!!!" << endl;
	cout << " Souce => " << souce << endl;
	cout << " Len => " << len << endl; 
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
		cout << " Souce => " << num << endl;
		cout << " Len => " << maxlen << "\n\n\n";
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
	Print(x, y, "∷", 0);
	Map[y][x] = 0;
	L++;
	if(!check())
		return GAMEOVER();
//	fstream log("log.txt", ios::app);
//	log << "{" << endl;
	x = head.x, y = head.y;
//	log << "[" << endl;
	Print(x, y, "■");
	snake[++R] = head;
//	log << "]" << endl;
	if(Map[y][x] < 0)
	{
//		log << "a" << endl;
		game.col = blue;
		souce += -1 * Map[y][x];
		Map[y][x] = 0;
		Print((M + 1) / 4 * 3, N, "￥", green);
		cnt += randint(1, 3);
		int tmp = souce;
		string Sou = "";
		for(; tmp; tmp /= 10)
			Sou = char((tmp % 10) + '0') + Sou;
		Print((M + 1) / 4 * 3 + 2, N, Sou, white);
		add = -1 * Map[y][x];
//		log << "b" << endl;
	}
	Map[y][x] = 3;
	if (cnt >= 3)
	{
//		log << "1" << endl;
		game.col = 5;
		cnt = 0;
		len++;
		L--;
		x = snake[L].x, y = snake[L].y;
		Print(x, y, "■");
		Map[y][x] = 3;
//		log << "2" << endl;
	}
//	log << "}" << endl;
//	log.close();
	if (R > 50000) R = 0, snake[R] = head;
	if (L > 50000) L = 0, snake[L] = head;
}

void entrance_main()
{
	head = snake[0];
	while(game.over != 1)
	{
//		while(game.over < 0);
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
		nowdir = dir;
	}
}

void init()
{
	srand(time(NULL));
	system("title Snake");//设置cmd窗口标题
//	system("mode con cols=61 lines=30");//窗口宽度高度
	game.col = white; 
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
	fstream f;
	f.open("map.txt");
	f >> N >> M;
	for (int i = 0; i < N; i++, printf("\n"))
	{
		cout << "  ";
		for (int j = 0; j < M; j++)
		{
			f >> Map[i][j];
			if (Map[i][j] > 0)
				kColor(white),
				cout << "█";
			else
				kColor(0),
				cout << "∷"; 
		}
	}
	f.close();
}

void entrance_creat_souce()
{
	srand(time(NULL));
	int x = 0, y = 0;
	while(game.over != 1)
	{
		Map[y][x] = 0;
		Print(x, y, "∷", 0);
		Sleep(game.speed * randint(1, 3));
//		while(game.over < 0); 
		x = randint(0, M - 1), y = randint(0, N - 1);
		while(Map[y][x] != 0) x = randint(0, M - 1), y = randint(0, N - 1);
		Map[y][x] = -1 * len * (randint(0, 5) == 0 ? randint(2, 3) : 1) * (200 / game.speed);
		Print(x, y, "￥", green);
		Sleep(game.speed * randint(int(1.0 * (N + M) * 1000 * 1 / 3), int (1.0 * (N + M) * 1000 * 5 / 3)) / 1000);
		
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
//			int LEN = now.s.size();
//			while(LEN--)
//				cout << "\b\b";
//			cout << now.s;
			for(char c : now.s)
				SetPos(now.x++, now.y),
				putchar(c);
		}
	}
}

void entrance_othre_draw()
{
//	while(game.over != 1)
//	{
//		if (game.col != white)
//		{
//			while(game.over < 0);
//			Sleep(game.speed * (len + 3));
//			game.col = white;
//		}
//	}
	while(game.over != 1)
	{
		while(game.col != white)
		{
			Print(M + 1, (N + 1) / 2, "!");
			while(game.over < 0);
			Sleep(game.speed * (len + 3));
			game.col = white;
			Print(M + 1, (N + 1) / 2, " ");
		}
	}
}

signed main()
{
	init();
	HideCursor();
	thread get_kb(entrance_get_kb);
	thread creat_souce(entrance_creat_souce);
	thread Draw(entrance_draw);
//	thread Other_Draw(entrance_othre_draw);
	entrance_main();
	get_kb.join();
	Draw.join();
//	Other_Draw.join(); 
	return 0; 
}
