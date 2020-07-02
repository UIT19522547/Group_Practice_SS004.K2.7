#include<Windows.h>﻿
#include<iostream>
#include<conio.h>
#include<cstdlib>
#include<time.h>
#include<fstream>
#include<stdlib.h>
#include<string>
#pragma execution_character_set( "utf-8" )
#define KB_UP 72
#define KB_DOWN 80
#define KB_LEFT 75
#define KB_RIGHT 77
#define KB_ESCAPE 27
#define width 60
#define height 20
#define Rong 25
#define Cao 7
#define CYAN 10
#define YELLOW 14

using namespace std;

int Score = 0;
int DoKho;

struct HighScore
{
	int Score;
	string Name;
};
HighScore highscore[5];
struct Point {
	int x;
	int y;
};
void XoaManHinh() {
	HANDLE hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD count;
	DWORD cellCount = 0;
	COORD homeCoords = { 0,0 };
	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	//Lấy số lượng ô trong bộ nhớ đệm hiện tại
	if (!GetConsoleScreenBufferInfo(hStdOut, &csbi))
		cellCount = csbi.dwSize.X * csbi.dwSize.Y;

	//Lấp đầy bộ nhớ đệm với khoảng trắng
	if (!FillConsoleOutputCharacter(hStdOut, (TCHAR)' ',
		cellCount, homeCoords, &count)) return;

	//Lấp đầy bộ nhớ đệm với màu và thuộc tính hiện tại
	if (!FillConsoleOutputAttribute(hStdOut, csbi.wAttributes
		, cellCount, homeCoords, &count)) return;

	//Di chuyển con trỏ về nhà
	SetConsoleCursorPosition(hStdOut, homeCoords);
}
void gotoXY(int column, int line) {
	COORD coord;
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void setTextColor(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
void DuaConTroVeDau()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos = { 0, 0 };
	SetConsoleCursorPosition(hConsole, pos);
}
void Write(string s, int x, int y, int color)//De in thong tin lua chon
{
	setTextColor(color);
	gotoXY(x, y); cout << s;
	setTextColor(15);
}
void Khung(int x1, int y1, int x2, int y2)
{
	int x, y;
	gotoXY(x1, y1); cout << "É";
	gotoXY(x2, y1); cout << "»";
	gotoXY(x1, y2); cout << "È";
	gotoXY(x2, y2); cout << "¼";
	for (x = x1 + 1; x < x2; x++)
	{
		gotoXY(x, y1); cout << "Í";
		gotoXY(x, y2); cout << "Í";
	}
	for (y = y1 + 1; y < y2; y++)
	{
		gotoXY(x1, y); cout << "º";
		gotoXY(x2, y); cout << "º";
	}
}
void Ve_menu(int x0, int y0, int chon, int n, string s[])
{
	y0 += 4;
	x0 += 10;
	system("cls");
	Write(" -----------------------------------------------\n", 24, 1, 7);
	Write("|                                               |\n", 24, 2, 7);
	Write("|    ******   *     *      *     *  *  *****    |\n", 24, 3, 7);
	Write("|   *         * *   *     * *    * *   *        |\n", 24, 4, 7);
	Write("|    ******   *  *  *    *   *   **    ****     | \n", 24, 5, 7);
	Write("|          *  *   * *   *******  * *   *        | \n", 24, 6, 7);
	Write("|    ******   *     *  *       * *  *  *****    |\n", 24, 7, 7);
	Write("|                                               |\n", 24, 8, 7);
	Write(" ----------------------------------------------- \n", 24, 9, 7);
	cout << "" << endl << endl;
	Khung(x0 - 2, y0 - 1, x0 + 30, y0 + n);
	for (int i = 0; i < n; i++)
		if (i == chon) Write(s[i], x0, y0 + i, CYAN);
		else Write(s[i], x0, y0 + i, YELLOW);
}
void endgame() //just some screens for certain actions
{
	cout << "" << endl << endl;
	cout << " ------------------------------------------------------------------------- " << endl;
	cout << "|    *****      *     *       * ******       ****  *       ****** ****    |" << endl;
	cout << "|   *          * *    * *   * * *           *    *  *     * *     *   *   |" << endl;
	cout << "|   *  ****   *   *   *  * *  * *****       *    *   *   *  ****  ****    |" << endl;
	cout << "|   *  *  *  *******  *   *   * *           *    *    * *   *     * *     |" << endl;
	cout << "|    *****  *       * *       * ******       ****      *    ***** *   *   |" << endl;
	cout << " ------------------------------------------------------------------------- " << endl;
	cout << "" << endl << endl;
	int tam;
	if (Score > highscore[0].Score) {
		tam = Score;
	}
	else (tam = highscore[0].Score);
	cout << "                        Y O U R   S C O R E : " << Score << endl << endl;
	cout << "                        H I G H   S C O R E : " << tam << endl;
	cout << "" << endl << endl;
}
class BACKGROUND {
public:
	void veKhung() {
		int j = 0;
		int i = 0;
		setTextColor(15);
		for (i = 0; i <= width; i += 2) {
			gotoXY(i, j);
			cout << "= ";
		}
		i = width;
		for (j = 1; j <= height; j++) {
			gotoXY(i, j);
			cout << "=";
		}
		i = 0;
		for (j = 1; j <= height; j++) {
			gotoXY(i, j);
			cout << "=";
		}
		j = height;
		for (i = 0; i <= width; i += 2) {
			gotoXY(i, j);
			cout << "= ";
		}
		setTextColor(14);

	}
	void drawFood(Point& F) {
		gotoXY(F.x, F.y);
		setTextColor(12);
		cout << "X";
		setTextColor(14);
		DuaConTroVeDau();
	}
};
class SNACK {
protected:
	Point ran[100];
	int doDai;
	int direction;
public:
	SNACK() {
		direction = 4;
		doDai = 3;
		ran[0].x = 10;
		ran[0].y = 10;
		ran[1].x = 11;
		ran[1].y = 10;
		ran[2].x = 12;
		ran[2].y = 10;
	}
	void Ve() {
		setTextColor(10);
		for (int i = 0; i < doDai; i++) {

			gotoXY(ran[i].x, ran[i].y);
			cout << "O";
		}
		setTextColor(14);
	}
	void Move() {
		for (int i = doDai - 1; i > 0; i--) {
			ran[i] = ran[i - 1];
		}
		if (direction == 1) {
			ran[0].y -= 1;
		}
		else if (direction == 2) {
			ran[0].y += 1;
		}
		else if (direction == 3) {
			ran[0].x -= 1;
		}
		else {
			ran[0].x += 1;
		}
	}
	void setDirection(int direct) {
		if (ran[0].x == ran[1].x && ran[0].y > ran[1].y && direct == 1);
		else if (ran[0].x == ran[1].x && ran[0].y < ran[1].y && direct == 2);
		else if (ran[0].y == ran[1].y && ran[0].x > ran[1].x && direct == 3);
		else if (ran[0].y == ran[1].y && ran[0].x < ran[1].x && direct == 4);
		else
			direction = direct;
	}
	//kiểm tra xem con rắn có tự đụng thân không
	bool checkCollision() {
		for (int i = 1; i < doDai; i++) {
			if ((ran[0].x == ran[i].x && ran[0].y == ran[i].y)) {
				return true;
			}
		}
		return false;
	}
	//Kiểm tra xem con rắn có đụng trúng khung hay không
	//Sử dụng hàm này khi chọn chế độ chơi không xuyên qua khung
	bool checkFrameConllision() {
		if (ran[0].x == 0 || ran[0].y == 0 || ran[0].x == width || ran[0].y == height)
			return true;
		return false;
	}
	//kiểm tra thức ăn có xuất hiện ở chỗ thân con rắn hiện tại hay không
	bool checkFoodCollision(Point F) {
		for (int i = 1; i < doDai; i++) {
			if (ran[i].x == F.x && ran[i].y == F.y)
				return true;
		}
		return false;
	}
	//Kiểm tra xem con rắn có ăn thức ăn hay chưa?
	bool ateFood(Point F) {
		if (ran[0].x == F.x && ran[0].y == F.y)
			return true;
		return false;
	}
	//Tăng độ dài của con rắn sau khi ăn mồi
	void growLength(Point F) {
		doDai++;
		for (int i = doDai - 1; i > 0; i--) {
			ran[i] = ran[i - 1];
		}
		ran[0] = F;
	}
	void goThroughWall() {
		//di chuyển đầu rắn sang phía bên không gian ngược lại
		if (direction == 1) {
			ran[0].y = height - 1;
		}
		else if (direction == 2) {
			ran[0].y = 1;
		}
		else if (direction == 3) {
			ran[0].x = width - 1;
		}
		else {
			ran[0].x = 1;
		}
	}
	int GetDoDai()
	{
		return this->doDai;
	}
};
class INTRODUCTION
{
public:
	void ChuThich(bool i) // 1-xuyên tường, 2-có tường 
	{
		system("color 0E");
		for (int j = 1; j < height; j += 2)
		{
			gotoXY(0, j);
			cout << "/";
			gotoXY(0, j + 1);
			cout << "\\";
			gotoXY(width, j);
			cout << "/";
			gotoXY(width, j - 1);
			cout << "\\";
		}
		for (int i = 0; i < width; i++)
		{
			gotoXY(i, height);
			cout << "-";
		}
		for (int i = 1; i <= width; i++)
		{
			gotoXY(i, 0);
			cout << "-";
		}
		gotoXY(2, 2);
		cout << "+ Hướng dẫn:";
		string s;
		gotoXY(2, 4);
		cout << "  Bạn phải sử dụng các phím mũi tên hoặc các phím W,A,S,D";
		gotoXY(2, 5);
		cout << "để di chuyển rắn và thu thập thức ăn. Bạn thu được càng ";
		gotoXY(2, 6);
		cout << "nhiều thức ăn thì rắn sẽ càng dài ra và điểm số càng tăng.";
		gotoXY(2, 7);
			cout << "Bạn có thể bấm phím SPACE để tạm dừng.";
		gotoXY(2, 8);
		cout << "";
		if (i == 1) //xuyên tường
		{
			gotoXY(2, 8);
			cout << "  Ở chế độ xuyên tường, bạn sẽ thua cuộc nếu như để cho ";
			gotoXY(2, 9);
			cout << "rắn tự cắn vào bất kì phần nào trên thân nó.";
			gotoXY(2, 11);
			cout << "+ Chú thích:";
			gotoXY(5, 13);
			setTextColor(10);
			cout << "OOOOO";
			setTextColor(14);
			cout << ": rắn";
			gotoXY(width / 2 + 5, 13);
			setTextColor(12);
			cout << "X";
			setTextColor(14);
			cout << ": thức ăn";
			gotoXY(5, 15);
		}
		else if (i == 0)
		{
			gotoXY(2, 8);
			cout << "  Ở chế độ không xuyên tường, bạn sẽ thua cuộc nếu như";
			gotoXY(2, 9);
			cout << "để cho rắn va vào tường hoặc tự cắn vào bất kì phần nào";
			gotoXY(2, 10);
			cout << "trên thân nó.";
			gotoXY(2, 12);
			cout << "+ Chú thích:";
			gotoXY(5, 14);
			setTextColor(10);
			cout << "OOOOO";
			setTextColor(14);
			cout << ": rắn";
			gotoXY(width / 2 + 5, 14);
			setTextColor(12);
			cout << "X";
			setTextColor(14);
			cout << ": thức ăn";
			gotoXY(5, 15);
			setTextColor(7);
			cout << "= = =";
			setTextColor(14);
			cout << ": tường";

		}
		DuaConTroVeDau();


	}
	int XuatChuThich(bool i) // trả về 1 là zô hàm play game, 0 là quay về menu 
	{
		int k = 2;
		system("cls");
		ChuThich(i);
		char ch = NULL;
		while (ch != 13)
		{
			switch (k)
			{
			case 1:
				gotoXY(9, 18);
				setTextColor(10);
				cout << "Quay lại";
				setTextColor(14);
				gotoXY(42, 18);
				cout << "Chơi";
				DuaConTroVeDau();
				ch = _getch();
				if (ch == KB_RIGHT)
					k = 2;
				if (ch == 13)
				{
					system("color 0F");
					return 0;
				}
				break;
			case 2:
				gotoXY(9, 18);
				cout << "Quay lại";
				gotoXY(42, 18);
				setTextColor(10);
				cout << "Chơi";
				setTextColor(14);
				DuaConTroVeDau();
				ch = _getch();
				if (ch == KB_LEFT)
					k = 1;
				if (ch == 13)
				{
					system("color 0F");
					return 1;
				}
				break;
			}
		}
	}
};
int PlayGame()
{
	srand(time(NULL));
	int KB_CODE = 0;
	bool xuyenTuong = 0;
	int Tam;
	//Vừa vào trò chơi yêu cầu người chơi chọn độ khó: dễ - trung bình - khó - siêu khó;
	int speed = 0;
	Khung(Rong - 2, Cao - 1, Rong + 50, Cao + 5);
	Write("\t    Chọn chế độ\n\t\t\t\t1-Classic\t\t  2-Modern\n", Rong + 10, Cao, YELLOW);
	cout << "\t\t\t\t\t\t "; cin >> Tam;
	if (Tam == 1) { xuyenTuong = 1; }
	if (Tam == 2) { xuyenTuong = 0; }
	Write("Độ Khó: 1-Dễ\t2-Trung bình\t3-Khó\t4-Siêu khó\n", Rong, Cao + 3, YELLOW);
	cout << "\t\t\t\t\t\t "; cin >> DoKho;
	switch (DoKho)
	{
	case 1: speed = 300; break;
	case 2: speed = 200; break;
	case 3: speed = 75; break;
	case 4: speed = 30; break;
	}
	INTRODUCTION I;
	int t;
	t = I.XuatChuThich(xuyenTuong);
	if (t == 0) { system("cls"); return 0; }
	//Mặc định hướng của con rắn ban đầu là đi qua phải
	int huong = 4;
	//Khai báo + tự động khởi tạo background và con rắn
	SNACK S;
	BACKGROUND B;
	S.Move(); //Lần đầu tiên di chuyển tọa độ đầu và đuôi sẽ bị trùng. Nếu không chạy hàm move 1 lần trước thì hàm checkCollision sẽ true -> Game Over ngay từ đầu
	//Ve khung và con rắn
	S.Ve();
	B.veKhung();
	//Khai báo và khởi tạo các tham số để quản lý con mồi
	long long timeFood = 0;
	bool FoodAppear = true;
	bool ateFood = 1;
	Point foodPoint;
	do {
		foodPoint.x = 1 + rand() % 59;
		foodPoint.y = 1 + rand() % 19;
	} while (S.checkFoodCollision(foodPoint));
	//Bắt đầu game
	while (KB_CODE != KB_ESCAPE) {
		XoaManHinh();
		system("cls");
		if (S.ateFood(foodPoint)) {
			Point T = foodPoint;
			do {
				foodPoint.x = 1 + rand() % 59;
				foodPoint.y = 1 + rand() % 19;
			} while (S.checkFoodCollision(foodPoint));
			B.drawFood(foodPoint);
			S.growLength(T);
		}
		else B.drawFood(foodPoint);
		timeFood++;
		if (_kbhit()) {
			KB_CODE = _getch();
			switch (KB_CODE) {
			case KB_UP: {
				huong = 1;
				break;
			}
			case KB_DOWN: {
				huong = 2;
				break;
			}
			case KB_LEFT: {
				huong = 3;
				break;
			}
			case KB_RIGHT: {
				huong = 4;
				break;
			}
			case KB_ESCAPE:
				break;
			case 32:
				B.veKhung();
				S.Ve();
				cout << "\n\n\n\n\n\n\n\n\n\n\n\n";
				system("pause");
				break;
			}
			S.setDirection(huong);
		}
		B.veKhung();
		if (xuyenTuong == 1 && S.checkFrameConllision()) {
			S.goThroughWall();
			S.Ve();
		}
		else {
			S.Move();
			if (S.checkFrameConllision());
			else
				S.Ve();
		}
		Score = S.GetDoDai() * DoKho * 10 - 3 * DoKho * 10;
		Write("Score: ", 62, 10, YELLOW);
		Write(to_string(Score), 69, 10, 15);
		Sleep(speed);
		//Xử lý Game Over trong chế độ không xuyên tường
		if (xuyenTuong == 1) {
			if (S.checkCollision()) {
				system("cls");
				endgame();
				break;
			}
		}
		else {
			if (S.checkCollision() || S.checkFrameConllision()) {
				system("cls");
				endgame();
				break;
			}
		}
	}
	return 1;
}
void SetEmpty()
{
	ofstream out("f:\\highscore.txt");
	for (int i = 0; i < 5; i++)
	{
		highscore[i].Name = "Player";
		highscore[i].Score = 0;
		out << highscore[i].Name << " - " << highscore[i].Score << endl;
	}
	out.close();
}
void ShowScore()
{
	int i = 0;
	ifstream in("f:\\highscore.txt");
	Khung(Rong - 2, Cao - 1, Rong + 30, Cao + 10);
	for (string str; getline(in, str);)
	{
		Write(str, Rong, Cao + i, 14);
		i++;
	}
	in.close();
}
void GetScore()
{
	ofstream out("f:\\highscore.txt");
	Write("Nhập tên: ", Rong + 25, Cao + 4, YELLOW);
	cin.ignore();
	if (highscore[0].Score == 0) { SetEmpty(); }
	for (int i = 4; i > 0; i--)
	{
		highscore[i].Name = highscore[i - 1].Name;
		highscore[i].Score = highscore[i - 1].Score;
	}
	getline(cin, highscore[0].Name);
	highscore[0].Score = Score;
	for (int i = 0; i < 5; i++)
	{
		if (highscore[i].Score == 0)
		{
			highscore[i].Name = "Player";
			highscore[i].Score = 0;
		}
		out << highscore[i].Name << " - " << highscore[i].Score << endl;
	}

	out.close();
}
void run()
{
	char ch;
	string st[20];
	system("cls");
	st[0] = "New Game";
	st[1] = "High Score";
	st[2] = "<ESC> Thoát game";
	int  chon = 0/*lua chon hien tai*/, luuchon/*lua chon truoc do*/, soluachon = 3, ok = FALSE/*Nhan enter hay chua*/;
	Ve_menu(Rong, Cao, chon, soluachon, st);
	do
	{
		ch = _getch(); //Nhan mot phim
		switch (ch)
		{
		case 72: //phim len
			luuchon = chon;
			chon--;
			if (chon < 0) chon = soluachon - 1;//Den cuoi thi bien dem quay lai lua chon dau
			Write(st[luuchon], Rong + 10, Cao + luuchon + 4, YELLOW);//lua chon truoc do doi lai thanh mau vang
			Write(st[chon], Rong + 10, Cao + chon + 4, CYAN);//lua chon dang chon se doi thanh mau xanh
			break;
		case 80://phim xuong
			luuchon = chon;
			chon++;
			if (chon == soluachon) chon = 0;
			Write(st[luuchon], Rong + 10, Cao + luuchon + 4, YELLOW);
			Write(st[chon], Rong + 10, Cao + chon + 4, CYAN);
			break;
		case 13: //phim ENTER
			ok = TRUE; break;
		}
		if (ok == TRUE) //Neu phim ENTER duoc nhan
		{
			switch (chon)
			{
			case 0:
				int PlayAgain;
				int brk;
			x1:
				system("cls");
				brk = PlayGame();
				if (brk == 1)
				{
					if (Score > highscore[0].Score) { GetScore(); }
					Write("Play again?", Rong + 2, Cao + 9, 14);
					Write("1-Yes\t      2-No\n", Rong - 4, Cao + 11, 15);
					cout << "\t\t\t      ";
					cin >> PlayAgain;
					if (PlayAgain == 1) goto x1;
				}
				Ve_menu(Rong, Cao, chon, soluachon, st);
				break;
			case 1:
				system("cls");
				ShowScore();
				int Xoa;
				Write("Xoá HighScore:\n\t\t\t 1-Có\t2-Không\n", Rong, Cao + 7, YELLOW);
				cout << "\t\t\t"; cin >> Xoa;
				if (Xoa == 1) { SetEmpty(); }
				cout << "\n\n" << "\t\t\t";
				system("pause");
				Ve_menu(Rong, Cao, chon, soluachon, st);
				break;
			case 2:
				system("cls");
				exit(0);
			}
			ok = FALSE; //tra lai trang thai ENTER chua duoc nhan
		}
	} while (ch != 27);//Nhan phim ESC de thoat khoi chuong trinh
}

int main()
{
	SetConsoleOutputCP(65001);
	run();
	return 0;
}
