//icon hozzáadása
//http://stackoverflow.com/questions/320677/how-do-i-set-the-icon-for-my-application-in-visual-studio-2008

#pragma once
#ifndef CONSOLEMANAGER_H
#define CONSOLEMANAGER_H

#include <iostream>
#include <windows.h>
#include <wincon.h>
#include <string>
#include <tchar.h>
#include <random>
#include <tuple>
#include <vector>
#include <thread>
#include <chrono>
#include <fstream>
#include <locale>
#include <codecvt>
#include <fcntl.h>
#include <io.h>
#include <ctime>
#include <chrono>
#include <algorithm>

using namespace std;

class ConsoleWindowManager {	
	
	const string ABOUT = "Axtros 2017";

	static const int CONSOLE_WINDOW_WIDTH = 1000;
	static const int CONSOLE_WINDOW_HEIGHT = 600;
	
	const string GAME_NAME = "Snake Game v.: 1.0";
	const string MENU_NEW_GAME = "New Game";
	const string MENU_EXIT = "Exit";
	const string RECORDS = "Records";
	const int SELECTED_MENU_COLOR = 14;
	const string RECORD_FILE = "rec.dat";
	const string PLAYER_POINT = "Pont: ";
	const bool IS_CENTER_SCREEN = true;	
	const string HORIZONTAL_LEVEL_WALL = "-";
	const string VERTICAL_LEVEL_WALL = "|";
	const int DEFAULT_CONSOLE_COLOR = 10;

	const int LEVEL_TOP = 2;
	const int LEVEL_LEFT = 10;
	const int LEVEL_WIDTH = 100;
	const int LEVEL_HEIGHT = 30;	

	const int SNAKE_HEAD_COLOR = 7;
	const int SNEAK_BODY_COLOR = 8;
	//const string SYMBOL = "■";
	//char b = '\u0444';
	//char a = 'ф';
	const string SNAKE_ELEMENT = "#";	
	const int STARTER_SNAKE_LENGTH = 4;
	const unsigned int GAME_SLEEP = 50;

	const string TARGET_ELEMENT = "#";
	const int TARGET_COLOR = 14;

	enum Modes {MENUMOD = 1, GAMEMOD = 2};
	typedef tuple<int, int> coord;				//http://www.oracle.com/technetwork/articles/servers-storage-dev/c-array-containers-2252536.html

	struct Record {
		string date;
		string point;		
		
		//https://www.codeproject.com/Articles/38381/STL-Sort-Comparison-Function
		bool operator < (const Record& rhs)
		{			
			int pointValue1 = stoi(point);		//hogy számként legyenek összehasonlítva, ne string-ként
			int pointValue2 = stoi(rhs.point);
			return pointValue1 > pointValue2;
			//return this-> pointValue1 < rhs.pointValue2;
		}
	};

private:
	HANDLE mHConsole; 
	HWND mhWnd;
	RECT mRect;
	COORD mCoord;
	unsigned int mMenuFirstLine;
	unsigned int mScreenWidth;
	unsigned int mScreenHeight;
	unsigned int mSnakeDirection;
	unsigned int mSnakeLength;
	unsigned int mPlayerPoint;	
	Modes modes;
	vector <coord> mSnake;
	vector <Record> mRecords;
	coord mTarget;
	void initConsoleWindow(unsigned int&, unsigned int&);
	void init(Modes&);
	void initAndCtrlMainMenu();
	void refreshMainMenu(const int&, bool);	
	void initStartPos();
	void initGameLayer();
	void initGame();
	void refreshGameArea();
	int gameLoop();
	void saveRecords();
	void loadRecords();
	void writeStrPos(const int& x, const int& y, const string& ch, const int& color);
	int getRandomNumber(const int& min, const int& max);
	bool isKeydown(const int& key);
	bool isWallCollision(const coord&);
	bool isTargetCollision(const coord& snakeHead, const coord& target);
	bool isSnakeCollision(const coord& snakeHead);
	void targetCollisionEvent();
	void readFile();
	bool mIsExistTarget;
	void generateTarget();
	void refreshPlayerPoint();
	//int compareRecords(const Record&, const Record&);
	//int compareRecords(const void* first, const void* second);
	struct recComperator;
public:
	ConsoleWindowManager();
	~ConsoleWindowManager();	
};

#endif // ! CONSOLEMANAGER_H

