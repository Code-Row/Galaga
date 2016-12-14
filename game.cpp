/*Gonna do the best I can to replicate Galaga.*/

#include <iostream>
#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;

int kbhit(){
	int ch = getch();
	if (ch != ERR){
		ungetch(ch);
		return 1;
	}
	else return 0;
}

class ship{
private:
	int position;
	int lives;
	int number;		//enemy
	int *positionX;
	int *positionY;
	bool *alive;
	bool *shot;
	int *shotX;
	int *shotY;
	int shooting;
	int score;
	int level;

public:
	void setLives(){
		lives = 3;		//3 lives and start in middle of the board
		position = 15;
		shot = new bool[30];
		shotX = new int[30];
		shotY = new int[30];
		shooting = 0;
		score = 0;
		level = 2;
	}

	int getLives(){
		return lives;
	}

	void shoot(){
		shotY[shooting] = 14;
		shotX[shooting] = position;
		shot[shooting] = true;
		shooting++;
		if (shooting == 30) shooting = 0;
	}

	void pewpew(){
		for (int i = 0; i < 30; i++){
			if (shot[i] == true){
				shotY[i]--;
				if (shotY[i] == 0) shot[i] = false;
				for (int j = 0; j < number; j++){
					if (alive[j] == true){
						if (positionX[j] == shotX[i] && positionY[j] == shotY[i]){
							alive[j] = false;
							shot[i] = false;
							number--;
							score += 50;
						}
					}
				}	
			}
		}
		if (((level - 1)*5) / number >= 2){
			configEnemy(level);
			level++;
		}
			
	}


	int moveshoot(){
		int move = getch();
		if (move == 'a'&& position > 0) position--;
		if (move == 'd' && position < 30) position++;	//board width = 30
		if (move == 'w') shoot();
		return position;
	}

	void printBoard(){
		clear();
		for (int i = 0; i < number; i++){
			if (alive[i] == true){	
				mvprintw(positionY[i], positionX[i], "#");
			}
		}  
		mvprintw(15, position, "<^>");		//probably a better shape for a ship but oh well
		for (int i = 0; i < 30; i++){
			if (shot[i] == true){
				mvprintw(shotY[i], shotX[i], "*");
			}
		}
		mvprintw(17, 0, "================================");
		mvprintw(18, 2, "Score: %d", score);
	}

	void configEnemy(int level){
		number = level * 5;				//need better system than this
		positionX = new int[number];
		positionY = new int [number];
		alive = new bool [number];

		for (int i = 0; i < number; i++){
			positionX[i] = rand() % 29 + 1;
			positionY[i] = rand() % 10;
			alive[i] = true;
		}
	}

	void moveEnemy(){
		for (int i = 0; i < number; i++){
			if (alive[i] == true){
				positionX[i] += (rand() % 3) - 1;
				if (positionX[i] <= 1) positionX[i]++;
				else if (positionX[i] >= 30) positionX--;
				positionY[i] += (rand() % 3) - 1;
				if (positionY[i] == 11) positionY[i]--;
				else if (positionY[i] == 0) positionY[i]++;
			}
		}
	}

};


int main(){
	WINDOW *win = initscr();
	cbreak();

	printw("You are a small fighter pilot. You ship <^> is under attack by a hoard of alien invaders.You must fight to survive.\nUse 'a' to move left, 'd' to move right, and 'w' to shoot. Press any key to start the game.\n");
	getch();
	clear();
	nodelay(win, true);
	ship a;
	a.setLives();
	int score = 0;
	int level = 1;
	a.configEnemy(level);

	while (a.getLives() > 0){
		if (kbhit() == 1){
			a.moveshoot();
		}
		a.moveEnemy();	
		a.pewpew();	
		a.printBoard();
		usleep(150000);
	}
	getch();
	endwin();
	return 0;
}
