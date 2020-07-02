
#include<Windows.h>﻿
#include<iostream>
#include<conio.h>
#include<cstdlib>
#include<time.h>
#include<fstream>
#include<stdlib.h>
#include<string>
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
//Tạo thức ăn cho rắn -> Tạo 1 hàm đếm cho 1 điều kiện nhất định để thức ăn xuất hiện
//Lưu trữ điểm: Tạo 1 file lưu trữ tên người chơi và điểm số sau mỗi màn để tạo bảng xếp hạng cho 3 người cao nhất!

using namespace std;
int Score = 0;
int DoKho;
//source: blogkhanhtoan.wordpress.com/2016/03/07/mot-so-ham-mo-rong-trong-cc/
//Hàm này lấy trên mạng mà không dùng được nên để đây, ai sửa được thì làm thử nha~
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

struct Point {
	int x;
	int y;
};

class BACKGROUND {
public:
	void veKhung() {
		int j = 0;
		int i = 0;
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

	}
	void drawFood(Point F) {
		gotoXY(F.x, F.y);
		cout << "*";
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
		for (int i = 0; i < doDai; i++) {
			gotoXY(ran[i].x, ran[i].y);
			cout << "X";
		}
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
int textcolor(int Color)
{
	HANDLE h;
	h = GetStdHandle(STD_OUTPUT_HANDLE);
	return SetConsoleTextAttribute(h, Color);
}
void Write(string s, int x, int y, int color)//De in thong tin lua chon
{
	textcolor(color);
	gotoXY(x, y); cout << s;
	textcolor(15);
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
	system("cls");
	Khung(x0 - 2, y0 - 1, x0 + 30, y0 + n);
	for (int i = 0; i < n; i++)
		if (i == chon) Write(s[i], x0, y0 + i, CYAN);
		else Write(s[i], x0, y0 + i, YELLOW);
}
void PlayGame() 
{
	srand(time(NULL));
	int KB_CODE = 0;
	bool xuyenTuong = 0;
	int Tam;
	//Vừa vào trò chơi yêu cầu người chơi chọn độ khó: dễ - trung bình - khó - siêu khó;
	int speed = 0;
	Write("Chon che do:\n\t\t\t 1-Classic\t2-Modern\n", Rong, Cao, YELLOW);
	cout << "\t\t\t      "; cin >> Tam;
	if (Tam == 1) { xuyenTuong = 1; }
	if (Tam == 2) { xuyenTuong = 0; }
	Write("Do kho: 1-De\t2-Trung binh\t3-Kho\t4-Sieu kho\n", Rong, Cao + 2, 15);
	cout << "\t\t\t      "; cin >> DoKho;
	switch (DoKho)
	{
	case 1: speed = 300; break;
	case 2: speed = 200; break;
	case 3: speed = 75; break;
	case 4: speed = 30; break;
	}
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
	bool FoodAppear = false;
	Point foodPoint;
	//Bắt đầu game
	while (KB_CODE != KB_ESCAPE) {
		XoaManHinh();
		system("cls");
		//Xử lý con mồi (thức ăn)
		if (timeFood > 1000000)
			timeFood = 1;
		if (timeFood % 5 == 0) {
			timeFood++;
			srand(time(NULL));
			FoodAppear = true;
			do {
				foodPoint.x = 1 + rand() % 59;
				foodPoint.y = 1 + rand() % 19;
			} while (S.checkFoodCollision(foodPoint));
		}
		if (FoodAppear == true) {
			B.drawFood(foodPoint);
			if (S.ateFood(foodPoint)) {
				S.growLength(foodPoint);
				FoodAppear = false;
			}
		}
		else
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
		Score = S.GetDoDai() * DoKho * 10 - S.GetDoDai() * 30;
		Sleep(speed);
		//Xử lý Game Over trong chế độ không xuyên tường
		if (xuyenTuong == 1) {
			if (S.checkCollision()) {
				Write("GAME OVER!", Rong, Cao, 15);
				break;
			}
		}
		else {
			if (S.checkCollision() || S.checkFrameConllision()) {
				Write("GAME OVER!", Rong, Cao, 15);
				break;
			}
		}
	}
}
struct HighScore
{
	int Score;
	string Name;
};

HighScore highscore[5];
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
	Write("Nhap ten: ", Rong + 37, Cao+2, YELLOW);
	cin.ignore();
	for (int i=4;i>0;i--)
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
	st[1] = "Hight Score";
	st[2] = "<ESC> Thoat game.";
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
			Write(st[luuchon], Rong, Cao + luuchon, YELLOW);//lua chon truoc do doi lai thanh mau vang
			Write(st[chon], Rong, Cao + chon, CYAN);//lua chon dang chon se doi thanh mau xanh
			break;
		case 80://phim xuong
			luuchon = chon;
			chon++;
			if (chon == soluachon) chon = 0;
			Write(st[luuchon], Rong, Cao + luuchon, YELLOW);
			Write(st[chon], Rong, Cao + chon, CYAN);
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
			x1:
				system("cls");
				PlayGame();
				if (Score >= highscore[0].Score) { GetScore();}
				Write("Play again?", Rong, Cao + 2, 14);
				Write("1-Yes\t2-No\n", Rong - 1, Cao + 4, 15);
				cout << "\t\t\t      ";
				cin >> PlayAgain;
				if (PlayAgain == 1) goto x1;
				Ve_menu(Rong, Cao, chon, soluachon, st);
				break;
			case 1:
				system("cls");
				ShowScore();
				int Xoa;
				Write("Xoa HighScore:\n\t\t\t 1-Co\t2-Khong\n", Rong, Cao + 7,YELLOW);
				cout << "\t\t\t"; cin >> Xoa;
				if (Xoa == 1) { SetEmpty(); }
				cout << "\n\n" << "\t\t\t" ;
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
	run();
	return 0;
}
