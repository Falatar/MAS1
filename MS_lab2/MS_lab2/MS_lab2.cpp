// MS_lab1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <fstream>

signed int space[9][16] = {
	{-1, -1,   -1,   -1,   -1,   -1, -100,   -1,   -1,   -1,   -1,   -1,   -1,   -1, -1,  -1},
	{-1, -1,   -1,   -1,   -1,   -1, -100,   -1,   -1,   -1,   -1,   -1,   -1,   -1, -1,  -1},
	{-1, -1,   -1,   -1,   -1,   -1, -100,   -1,   -1,   -1,   -1,   -1,   -1, -100, -1,  -1},
	{-1, -1,   -1,   -1,   -1,   -1, -100,   -1,   -1,   -1,   -1,   -1,   -1, -100, -1,  -1},
	{-1, -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, -100, -1,  -1},
	{-1, -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, -100, -1,  -1},
	{-1, -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, -100, -1,  -1},
	{-1, -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1, -100, -1,  -1},
	{-1, -1, -100, -100, -100, -100, -100, -100, -100, -100, -100, -100, -100, -100, -1, 100}
};

struct agent {
	char* name;
	std::vector<char> way;
	signed int reward;
	int coord_Y;
	int coord_X;
	int broCoord_X;
	int broCoord_Y;
	double picture[9][16][5];
	bool map[9][16];
};

double LF = 0.5; //фактор обучения
double DF = 0.5; //Фактор дисконтирования

int main()
{
	using namespace std;
	srand(time(0));
	bool complt = false;
	agent agent_lab1, agent_lab2;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 16; j++) {
			if (j == 15) { agent_lab1.picture[i][j][0] = INT_MIN; agent_lab2.picture[i][j][0] = INT_MIN; }
			else if (j == 14) { agent_lab1.picture[i][j][0] = 0; agent_lab2.picture[i][j][0] = INT_MIN; }
			else { agent_lab1.picture[i][j][0] = 0;  agent_lab2.picture[i][j][0] = 0; }
			if (i == 8) { agent_lab1.picture[i][j][1] = INT_MIN; agent_lab2.picture[i][j][1] = INT_MIN; }
			else if (i == 7) { agent_lab1.picture[i][j][1] = 0; agent_lab2.picture[i][j][1] = INT_MIN; }
			else { agent_lab1.picture[i][j][1] = 0;  agent_lab2.picture[i][j][1] = 0; }
			if (j == 0) { agent_lab1.picture[i][j][2] = INT_MIN; agent_lab2.picture[i][j][2] = INT_MIN; }
			else if (j == 1) { agent_lab1.picture[i][j][2] = 0; agent_lab2.picture[i][j][2] = INT_MIN; }
			else { agent_lab1.picture[i][j][2] = 0;  agent_lab2.picture[i][j][2] = 0; }
			if (i == 0) { agent_lab1.picture[i][j][3] = INT_MIN; agent_lab2.picture[i][j][3] = INT_MIN; }
			else if (i == 1) { agent_lab1.picture[i][j][3] = 0; agent_lab2.picture[i][j][3] = INT_MIN; }
			else { agent_lab1.picture[i][j][3] = 0;  agent_lab2.picture[i][j][3] = 0; }
			agent_lab1.picture[i][j][4] = 0;
			agent_lab2.picture[i][j][4] = 0;
		}
	}
	bool mul[9][16];
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 16; j++) {
			mul[i][j] = 0;
		}
	}
	ofstream fout;
	fout.open("log.txt");
	int counter = 0;
	signed last_rew = 0;
	int cond = 0;
	while (true) {
		agent_lab1.coord_Y = 8;
		agent_lab1.coord_X = 0;
		agent_lab1.broCoord_Y = 8;
		agent_lab1.broCoord_X = 1;
		agent_lab1.reward = 0;
		agent_lab1.way.clear();
		agent_lab2.coord_Y = 8;
		agent_lab2.coord_X = 1;
		agent_lab2.broCoord_Y = 8;
		agent_lab2.broCoord_Y = 0;
		agent_lab2.reward = 0;
		agent_lab2.way.clear();
		//Ускорение
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 16; j++) {
				agent_lab1.map[i][j] = false;
			}
		}
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 16; j++) {
				agent_lab2.map[i][j] = false;
			}
		}
		agent_lab1.map[agent_lab1.coord_Y][agent_lab1.coord_X] = true;
		agent_lab2.map[agent_lab2.coord_Y][agent_lab2.coord_X] = true;
		//Ускорение
		while (true) {
			//ARGMAX
			double max_val = INT_MIN;
			int next = 0;
			for (int i = 0; i < 5; i++) {
				if (agent_lab1.picture[agent_lab1.coord_Y][agent_lab1.coord_X][i] > max_val) {
					max_val = agent_lab1.picture[agent_lab1.coord_Y][agent_lab1.coord_X][i];
					next = i;
				}
			}
			//ARGMAX
			int last_Y = agent_lab1.coord_Y;
			int last_X = agent_lab1.coord_X;
			if (next == 0) {
				agent_lab1.coord_X++;
				agent_lab1.way.push_back('r');
			}
			else if (next == 1) {
				agent_lab1.coord_Y++;
				agent_lab1.way.push_back('d');
			}
			else if (next == 2) {
				agent_lab1.coord_X--;
				agent_lab1.way.push_back('l');
			}
			else if (next == 3) {
				agent_lab1.coord_Y--;
				agent_lab1.way.push_back('u');
			}
			else agent_lab1.way.push_back('s');
			agent_lab1.reward += space[agent_lab1.coord_Y][agent_lab1.coord_X];
			int max = INT_MIN;
			for (int i = 0; i < 5; i++) {
				if (agent_lab1.picture[agent_lab1.coord_Y][agent_lab1.coord_X][i] > max) {
					max = agent_lab1.picture[agent_lab1.coord_Y][agent_lab1.coord_X][i];
				}
			}
			//Обновление данных
			agent_lab1.picture[last_Y][last_X][next] += LF * (space[agent_lab1.coord_Y][agent_lab1.coord_X] + DF * max - agent_lab1.picture[last_Y][last_X][next]);
			if (agent_lab1.map[agent_lab1.coord_Y][agent_lab1.coord_X] == true) agent_lab1.picture[last_Y][last_X][next] -= 1;
			agent_lab1.map[agent_lab1.coord_Y][agent_lab1.coord_X] = true;
			//Обновление данных

			for (int i = 0; i < 5; i++) {
				int X = agent_lab2.broCoord_X, Y = agent_lab2.broCoord_Y, X1 = agent_lab2.coord_X, Y1 = agent_lab2.coord_Y;
				if (i == 0) {
					X1 += 2;
				}
				else if (i == 1) {
					Y1 += 2;
				}
				else if (i == 2) {
					X1 -= 2;
				}
				else if (i == 3) {
					Y1 -= 2;
				}
				if (abs(X - X1) > 2 || abs(Y - Y1) > 2) agent_lab2.picture[agent_lab2.coord_Y][agent_lab2.coord_X][i] -= 100;
				else if (abs(X - X1) > 1 || abs(Y - Y1) > 1) agent_lab2.picture[agent_lab2.coord_Y][agent_lab2.coord_X][i] -= 100;
				if (X == X1 && Y == Y1) agent_lab2.picture[agent_lab2.coord_Y][agent_lab2.coord_X][i] -= 1;
			}
			//ARGMAX 2
			max_val = INT_MIN;
			next = 0;
			for (int i = 0; i < 5; i++) {
				cout << agent_lab2.picture[agent_lab2.coord_Y][agent_lab2.coord_X][i] << " ";
				if (agent_lab2.picture[agent_lab2.coord_Y][agent_lab2.coord_X][i] > max_val) {
					max_val = agent_lab2.picture[agent_lab2.coord_Y][agent_lab2.coord_X][i];
					next = i;
				}
			}
			cout << endl;
			//ARGMAX 2
			for (int i = 0; i < 5; i++) {
				int X = agent_lab2.broCoord_X, Y = agent_lab2.broCoord_Y, X1 = agent_lab2.coord_X, Y1 = agent_lab2.coord_Y;
				if (i == 0) {
					X1 += 2;
				}
				else if (i == 1) {
					Y1 += 2;
				}
				else if (i == 2) {
					X1 -= 2;
				}
				else if (i == 3) {
					Y1 -= 2;
				}
				if (abs(X - X1) > 2 || abs(Y - Y1) > 2) agent_lab2.picture[agent_lab2.coord_Y][agent_lab2.coord_X][i] += 100;
				else if (abs(X - X1) > 1 || abs(Y - Y1) > 1) agent_lab2.picture[agent_lab2.coord_Y][agent_lab2.coord_X][i] += 100;
				if (X == X1 && Y == Y1) agent_lab2.picture[agent_lab2.coord_Y][agent_lab2.coord_X][i] += 1;
			}
			last_Y = agent_lab2.coord_Y;
			last_X = agent_lab2.coord_X;
			if (next == 0) {
				agent_lab2.coord_X += 2;
				agent_lab2.way.push_back('r');
			}
			else if (next == 1) {
				agent_lab2.coord_Y += 2;
				agent_lab2.way.push_back('d');
			}
			else if (next == 2) {
				agent_lab2.coord_X -= 2;
				agent_lab2.way.push_back('l');
			}
			else if (next == 3) {
				agent_lab2.coord_Y -= 2;
				agent_lab2.way.push_back('u');
			}
			else agent_lab2.way.push_back('s');
			agent_lab2.reward += space[agent_lab2.coord_Y][agent_lab2.coord_X];
			max = INT_MIN;
			for (int i = 0; i < 5; i++) {
				if (agent_lab2.picture[agent_lab2.coord_Y][agent_lab2.coord_X][i] > max) {
					max = agent_lab2.picture[agent_lab2.coord_Y][agent_lab2.coord_X][i];
				}
			}
			//Обновление данных 2
			int mulct = 0;
			agent_lab2.picture[last_Y][last_X][next] += LF * (space[agent_lab2.coord_Y][agent_lab2.coord_X] + DF * max - agent_lab2.picture[last_Y][last_X][next]);
			if (abs(agent_lab1.coord_Y - agent_lab2.coord_Y) > 2 || abs(agent_lab1.coord_X - agent_lab2.coord_X) > 2) {
				mulct = 7;
			}
			agent_lab2.map[agent_lab2.coord_Y][agent_lab2.coord_X] = true; //rrr
			agent_lab1.broCoord_Y = agent_lab2.coord_Y;
			agent_lab1.broCoord_X = agent_lab2.coord_X;
			agent_lab2.broCoord_Y = agent_lab1.coord_Y;
			agent_lab2.broCoord_X = agent_lab1.coord_X;
			//Обновление данных 2

			if (agent_lab1.coord_X == agent_lab2.coord_X && agent_lab1.coord_Y == agent_lab2.coord_Y) {
				mul[agent_lab1.coord_Y][agent_lab1.coord_X] = 1;
				agent_lab1.reward--;
				agent_lab2.reward--;
			}
			if (space[agent_lab1.coord_Y][agent_lab1.coord_X] == -100 || space[agent_lab2.coord_Y][agent_lab2.coord_X] == -100) break;
			if (space[agent_lab1.coord_Y][agent_lab1.coord_X] == 100 || space[agent_lab2.coord_Y][agent_lab2.coord_X] == 100) {
				complt = true;
				break;
			}
			if (mulct) {
				agent_lab1.reward -= 100;
				agent_lab2.reward -= 100;
				cout << "BOOOB " << abs(agent_lab1.coord_Y - agent_lab2.coord_Y) << " " << abs(agent_lab1.coord_X - agent_lab2.coord_X) << " ";
				break;
			}
		}
		fout << counter << " " << agent_lab1.reward << " " << agent_lab2.reward << " ";
		for (int i = 0; i < agent_lab1.way.size(); i++) {
			fout << agent_lab1.way[i];
		}
		fout << " ";
		for (int i = 0; i < agent_lab2.way.size(); i++) {
			fout << agent_lab2.way[i];
		}
		fout << endl;
		std::cout << counter << "   ||   " << agent_lab1.reward << "   ||   " << agent_lab2.reward << endl;
		counter++;
		if (agent_lab1.reward == last_rew) cond++;
		else cond = 0;
		last_rew = agent_lab1.reward;
		if (complt == true) break;
		if (cond == 1000) {
			cout << "COMPL";
			break;
		}//условие выхода
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 16; j++) {
				mul[i][j] = 0;
			}
		}
	}
	cout << endl << endl;
	for (int i = 0; i < 9; i++) {
		cout << "   ";
		for (int j = 0; j < 16; j++) {
			cout << agent_lab1.map[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl << endl;
	for (int i = 0; i < 9; i++) {
		cout << "   ";
		for (int j = 0; j < 16; j++) {
			cout << agent_lab2.map[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl << endl;
	for (int i = 0; i < 9; i++) {
		cout << "   ";
		for (int j = 0; j < 16; j++) {
			cout << mul[i][j] << " ";
		}
		cout << endl;
	}
	fout.close();
}

void fixCoord(agent& agnt) {

}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

