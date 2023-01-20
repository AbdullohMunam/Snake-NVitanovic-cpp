#include <iostream>
#include <ncurses.h>
#include <unistd.h>
#include <chrono>
#include <thread>

// for kbhit
#include <sys/ioctl.h>
#include <termios.h>
bool kbhit()
{
    termios term;
    tcgetattr(0, &term);

    termios term2 = term;
    term2.c_lflag &= ~ICANON;
    tcsetattr(0, TCSANOW, &term2);

    int byteswaiting;
    ioctl(0, FIONREAD, &byteswaiting);

    tcsetattr(0, TCSANOW, &term);

    return byteswaiting > 0;
}


bool gameOver;
const int width = 40;
const int height = 20;
int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int nTail; // panjang Tail
enum eDirection{STOP = 0, LEFT, RIGHT, UP, DOWN};
eDirection dir;


using namespace std;

void setup(){
	gameOver = false;
	dir = STOP;
	x = width / 2;
	y = height / 2;
	fruitX = rand() % width;
	fruitY = rand() % height;
	score = 0;
}

void draw(){ 
	system("clear"); // on Windows system("cls")
	for (int i = 0; i < width+2; i++)
		cout << "#";
	cout << endl;

	for (int i = 0; i < height; i++){
		for (size_t j = 0; j < width; j++)
		{
			if (j == 0)
				cout << "#";
			if (i == y && j == x)
				cout << "O";
			else if (i == fruitY && j == fruitX)
				cout << "F";
			else {
				bool print = false;
				
				for (int k = 0; k <nTail; k++){

					if (tailX[k] == j && tailY[k] == i){
						cout << "o";
						print = true;
					}
				}
				if (!print)
					cout << " ";
			}

			if (j == width -1)
				cout << "#";
		}
		cout << endl;
		
	}

	for (int i = 0; i < width+2; i++)
		cout << "#";
	cout << endl;

	// score
	cout << "w=UP, a=LEFT, s=DOWN, d=RIGHT, x=EXIT";
	cout << "  Score : " << score << endl;
	// cout << "Press x to exit";
}

void input(){
	if (kbhit()){
		switch (cin.get()) // karena tidak bisa menggunakan getch() maka menggunakan cin.get()
		{
		case 'w':
			if (dir == DOWN);
			else dir = UP;
			break;
		case 'a':
			if (dir == RIGHT);
			else dir = LEFT;
			break;
		case 's':
			if (dir == UP);
			else dir = DOWN;
			break;
		case 'd':
			if (dir == LEFT);
			else dir = RIGHT;
			break;
		case 'x' || 'X':
			gameOver = true;
			break;
		}
	}
}

void logic(){
	int prevX = tailX[0];
	int prevY = tailY[0];
	int prev2X, prev2Y;
	tailX[0] = x;
	tailY[0] = y;

	for(int i = 1; i < nTail; i++){
		prev2X = tailX[i];
		prev2Y = tailY[i];
		tailX[i] = prevX;
		tailY[i] = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}

	switch (dir){
		case LEFT:
			x--;
			break;
		case RIGHT:
			x++;
			break;
		case UP:
			y--;
			break;
		case DOWN:
			y++;
			break;
		default:
			break;
	}

	// mati jika menabrak tembok
	if (x > width || x < 0 || y > height || y < 0)
		gameOver = true;
	
	// bisa melewati tembok
	// if (x >= width) x = 0; else if (x < 0) x = width - 1;
	// if (y >= height) y = 0; else if (y < 0) y = height -1;


	// mati jika menabrak diri sendiri
	for (int i = 0; i < nTail; i++)
		if (tailX[i] == x && tailY[i] == y)
			gameOver = true;

	if(x == fruitX && y == fruitY){
		score += 10;
		fruitX = rand() % width;
		fruitY = rand() % height;
		nTail++;
	}
}

int main(int argc, char const *argv[])
{
	setup();
	while (!gameOver)
	{
		draw();
		input();
		logic();
		// sleep(1); // hanya untuk seconds
		std::this_thread::sleep_for(std::chrono::milliseconds(200)); // untuk milisecons
	}
	
	return 0;
}
