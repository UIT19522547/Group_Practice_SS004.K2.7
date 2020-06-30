#include<iostream>
#include<Windows.h>
#include<conio.h>
#include<cstdlib>
#include<time.h>
#include<fstream>
#include<stdlib.h>
#define KB_UP 72
#define KB_DOWN 80
#define KB_LEFT 75
#define KB_RIGHT 77
#define KB_ESCAPE 27
#define width 60
#define height 20

//Tạo thức ăn cho rắn -> Tạo 1 hàm đếm cho 1 điều kiện nhất định để thức ăn xuất hiện
//Lưu trữ điểm: Tạo 1 file lưu trữ tên người chơi và điểm số sau mỗi màn để tạo bảng xếp hạng cho 3 người cao nhất!

using namespace std;

//source: blogkhanhtoan.wordpress.com/2016/03/07/mot-so-ham-mo-rong-trong-cc/
//Hàm này lấy trên mạng mà không dùng được nên để đây, ai sửa được thì làm thử nha~
void XoaManHinh() {
	HANDLE hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD count;
	DWORD cellCount=0;
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
		for (i = 0; i <= width ; i += 2) {
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
		j = height ;
		for (i = 0; i <= width; i+=2) {
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
			ran[i] = ran[i-1];
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
		for (int i = 1; i <doDai ; i++) {
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
};

void PlayGame() {
	srand(time(NULL));
	int KB_CODE = 0;
	//Mặc định hướng của con rắn ban đầu là đi qua phải
	int huong = 4;
	//Vừa vào trò chơi yêu cầu người chơi chọn độ khó: dễ - trung bình - khó - siêu khó;
	int doKho;
	int speed;
	//Chọn chế độ: xuyên tường hay không?
	bool xuyenTuong;
	//Khai báo + tự động khởi tạo background và con rắn
	SNACK S;
	BACKGROUND B;
	S.Move(); //Lần đầu tiên di chuyển tọa độ đầu và đuôi sẽ bị trùng. Nếu không chạy hàm move 1 lần trước thì hàm checkCollision sẽ true -> Game Over ngay từ đầu
	//Ve khung và con rắn
	S.Ve();
	B.veKhung();
	//Quản lý điểm số
	string score = "0";
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
			}
			S.setDirection(huong);
		}
		B.veKhung();
		S.Ve();
		S.Move();
		Sleep(200);
		//Xử lý Game Over
		if (S.checkCollision() || S.checkFrameConllision()) {
			gotoXY(width / 2, height / 2);
			cout << "GAME OVER!";
			break;
		}
	}
}

int main() {
	
	return 0;
}

