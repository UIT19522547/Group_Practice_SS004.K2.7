#include "windows.h"
#include <iostream>
#include <conio.h>

using namespace std;
#define CYAN 10
#define YELLOW 14
void gotoxy(int x, int y)
{
	printf("\033[%dG\033[%dd", x + 1, y + 1);
}
int textcolor(int Color)
{
	HANDLE h;
	h = GetStdHandle(STD_OUTPUT_HANDLE);
	return SetConsoleTextAttribute(h, Color);
}
void Khung(int x1, int y1, int x2, int y2)
{
	int x, y;
	gotoxy(x1, y1); cout << "É";
	gotoxy(x2, y1); cout << "»";
	gotoxy(x1, y2); cout << "È";
	gotoxy(x2, y2); cout << "¼";
	for (x = x1 + 1; x < x2; x++)
	{
		gotoxy(x, y1); cout << "Í";
		gotoxy(x, y2); cout << "Í";
	}
	for (y = y1 + 1; y < y2; y++)
	{
		gotoxy(x1, y); cout << "º";
		gotoxy(x2, y); cout << "º";
	}
}
void Write(string s, int x, int y, int color)
{
	textcolor(color);
	gotoxy(x, y); cout << s;
	textcolor(15);
}
void Ve_menu(int x0, int y0, int chon, int n, string s[])
{
	system("cls");
	Khung(x0 - 2, y0 - 1, x0 + 30, y0 + n);
	for (int i = 0; i < n; i++)
		if (i == chon) Write(s[i], x0, y0 + i, CYAN);
		else Write(s[i], x0, y0 + i, YELLOW);
}

int main()
{
	char ch;
	string st[20];
	system("cls");
	st[0] = "New Game";
	st[1] = "Hight Score";
	st[2] = "<ESC> Thoat game.";
	int Cao = 25, Rong = 10, chon = 0, luuchon, soluachon = 3, ok = FALSE;
	Ve_menu(Cao, Rong, chon, soluachon, st);
	do
	{
		ch = _getch(); //Nhan mot phim
		switch (ch)
		{
		case 72: //phim len
			luuchon = chon;
			chon--;
			if (chon < 0) chon = soluachon - 1;
			Write(st[luuchon], Cao, Rong + luuchon, YELLOW);
			Write(st[chon], Cao, Rong + chon, CYAN);
			break;
		case 80://phim xuong
			luuchon = chon;
			chon++;
			if (chon == soluachon) chon = 0;
			Write(st[luuchon], Cao, Rong + luuchon, YELLOW);
			Write(st[chon], Cao, Rong + chon, CYAN);
			break;
		case 13: //phim ENTER
			ok = TRUE; break;
		}
		if (ok == TRUE) //Neu phim ENTER duoc nhan
		{
			switch (chon)
			{
			case 0:
				system("cls");
				cout << "New Game";
				exit(0);
				break;
			case 1:
				system("cls");
				cout << "Hight Score";
				exit(0);
				break;
			case 2:
				system("cls");
				exit(0);
			}
			ok = FALSE; //tra lai trang thai ENTER chua duoc nhan
		}
	} while (ch != 27);//Nhan phim ESC de thoat khoi chuong trinh
	return 0;
}
