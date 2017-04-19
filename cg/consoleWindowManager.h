#pragma once
#ifndef CONSOLEMANAGER_H
#define CONSOLEMANAGER_H

#include <windows.h>
#include <string>
#include <tchar.h>
#include <random>
#include <tuple>
#include <vector>
#include <thread>
#include <chrono>

using namespace std;

class ConsoleWindowManager {	
	
	static const int CONSOLE_WINDOW_WIDTH = 1000;
	static const int CONSOLE_WINDOW_HEIGHT = 600;
	
	const string GAME_NAME = "Snake Game v.: 1.0";	
	const bool IS_CENTER_SCREEN = true;	
	const string HORIZONTAL_LEVEL_WALL = "-";
	const string VERTICAL_LEVEL_WALL = "|";	
	const int DEFAULT_CONSOLE_COLOR = 10;

	const int LEVEL_TOP = 2;
	const int LEVEL_LEFT = 10;
	const int LEVEL_WIDTH = 100;
	const int LEVEL_HEIGHT = 30;	

	const string SNAKE_ELEMENT = "#";

	typedef tuple<int, int> snakeCoord;				//http://www.oracle.com/technetwork/articles/servers-storage-dev/c-array-containers-2252536.html
	
private:
	HANDLE mHConsole;
	HWND mhWnd;
	RECT mRect;
	COORD mCoord;
	int mScreenWidth;
	int mScreenHeight;
	int mSnakeDirection;
	vector <snakeCoord> mSnake;
	void init(int&, int&);
	void initStartPos();
	void initLayer();
	void initGame();
	void writeSnake();
	int gameLoop();
	void writeStrPos(const int& x, const int& y, const string& ch, const int& color);
	int getRandomNumber(const int& min, const int& max);
	bool isKeydown(const int& key);
	bool isWallCollision(const snakeCoord&);

public:
	ConsoleWindowManager();
	~ConsoleWindowManager();	
};

#endif // ! CONSOLEMANAGER_H

