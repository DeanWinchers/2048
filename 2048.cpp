#include <string>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <curses.h>

using namespace std;

#define N 4

#define WIDTH 5

#define TARGET 2048

//state of game
#define FAIL 0
#define WIN 1
#define NORMAL 2
#define QUIT 3

class Game2048{
public:
	Game2048() : status(NORMAL) {
		start();
	}

	int getStatus(){ return status; }

	void processInput() {
		char c = getch();
		if(c >= 'a' && c <= 'z')
			c -= 32;
		if(status == NORMAL) {
			bool updated;
			switch(c){
			case 'A':
				updated = moveToLeft();
				break;
			case 'S':
				rotate();
				rotate();
				rotate();
				updated = moveToLeft();
				rotate();
				break;
			case 'D':
				rotate();
				rotate();
				updated = moveToLeft();
				rotate();
				rotate();
				break;
			case 'W':
				rotate();
				updated = moveToLeft();
				rotate();
				rotate();
				rotate();
				break;
			}
			if(updated){
				randNum();
			}
		}

		if(c == 'Q')
			status = QUIT;
		else if(c == 'R')
			start();
	}

	void draw()
	{
		clear();
		int offset = 12;
		for(int i = 0; i <= N; ++i){
			for(int j = 0; j <= N; ++j){
				drawItem(i * 2, j * WIDTH + offset, '+');
				if(i != N)
					drawItem(i * 2 + 1, j * WIDTH + offset, '|');
				if(j != N)
					for(int k = 1; k <= WIDTH; ++k){
						drawItem(i * 2, j * WIDTH + offset + k, '-');
					}
				if(i != N && j != N)
					drawNum(2 * i + 1, j * WIDTH + offset + WIDTH - 1, data[i][j]);
			}
		}
		mvprintw(2 * N + 2, 0, "W(UP),S(DOWN),A(LEFT),D(RIGHT),R(RESTART),Q(QUIT)");
		mvprintw(2 * N + 3, 16, "https://nothing");

		if(status == WIN)
			mvprintw( N, 5 * N / 2 - 1, " YOU WIN,PRESS R TO CONTINUE ");
		else if(isOver())
			mvprintw( N, 5 * N / 2 - 1, " YOU LOSE,PRESS R TO CONTINUE ");
	}
private:
	void drawItem(int row, int col, char c){
		move(row, col);
		addch(c);
	}

	void drawNum(int row, int col, int n){
		while(n > 0){
			drawItem(row, col, n % 10 + '0');
			n /= 10;
			--col;
		}
	}

	void randNum(){
		vector<int> empty;
		for(int i = 0; i < N; ++i)
                        for(int j = 0; j < N; ++j){
				if(data[i][j] == 0)
					empty.push_back(i * N + j);
			}
		int pos = empty[rand() % empty.size()];
		data[pos / N][pos % 4] = rand() % 10 == 1 ? 4 : 2;
	}

	void start(){
		for(int i = 0; i < N; ++i)
			for(int j = 0; j < N; ++j)
				data[i][j] = 0;
		randNum();
		randNum();

	}

	void rotate(){
		int tmp[N][N] = {0};
		for(int i = 0; i < N; ++i)
			for(int j = 0; j < N; ++j)
				tmp[N - 1 - j][i] = data[i][j];
		for(int i = 0; i < N; ++i)
                        for(int j = 0; j < N; ++j)
				data[i][j] = tmp[i][j];
	}

	bool moveToLeft(){
		int tmp[N][N];
		for(int i = 0; i < N ; ++i){
			int lastVal = 0;
			int currentWriterPos = 0;
                        for(int j = 0; j < N; ++j) {
				tmp[i][j] = data[i][j];

				if(data[i][j] == 0)
					continue;
				if(lastVal == 0) {
					lastVal = data[i][j];
				}
				else {
					if(lastVal == data[i][j]){
						data[i][currentWriterPos] = lastVal * 2;
						lastVal = 0;
						if (data[i][currentWriterPos] == TARGET)
							status = WIN;
					} else {
						data[i][currentWriterPos] = lastVal;
						lastVal = data[i][j];
					}
					++currentWriterPos;
				}
				data[i][j] = 0;
			}
			if(lastVal != 0)
				data[i][currentWriterPos] = lastVal;
		}

		for(int i = 0; i < N; ++i)
			for (int j = 0; j < N; ++j) {
                		if (data[i][j] != tmp[i][j]) return true;
            	}
		return false;
	}

	bool isOver(){
		for(int i = 0; i < N ; ++i)
			for(int j = 0; j < N; ++j){
				if((1 + j < N) && (data[i][j] == 0 || data[i][j] == data[i][j + 1]))
					return false;
				if((1 + i < N) && (data[i][j] == data[i + 1][j]))
					return false;
			}
		return true;
	}
	int data[N][N];
	int status;
};

void initialize() {
        // initialize ncurses
        initscr();
        //interactive has no need to press carriage return character
        cbreak();
        //press character show nothing
        noecho();
        //hide cursor
        curs_set(0);
        //random number
        srand(time(NULL));
}

void shutdown(){
        //clear ncurses
        endwin();
}

int main()
{
	initialize();
	Game2048 game;
	do{
		game.draw();
		game.processInput();
	} while(game.getStatus() != QUIT);

	shutdown();
	return 0;
}
