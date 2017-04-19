//http://en.cppreference.com/w/

#include <iostream>
#include "consoleWindowManager.h"

using namespace std;

ConsoleWindowManager::ConsoleWindowManager() {
	//cout << "Object is being created" << endl;
	mHConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	init(mScreenWidth, mScreenHeight);
	initLayer();
	initGame();
}

ConsoleWindowManager::~ConsoleWindowManager() {
	//cout << "Object is being deleted" << endl;
}

void ConsoleWindowManager::init(int& mScreenWidth, int& mScreenHeight)
{		
	mhWnd = GetDesktopWindow();
	GetWindowRect(mhWnd, &mRect);
	mScreenWidth = mRect.right;
	mScreenHeight = mRect.bottom;
	mhWnd = GetConsoleWindow();
	if (IS_CENTER_SCREEN) {
		int consoleWindowX = ((mScreenWidth / 2) - (CONSOLE_WINDOW_WIDTH / 2));
		int consoleWindowY = ((mScreenHeight / 2) - (CONSOLE_WINDOW_HEIGHT / 2));		
		MoveWindow(mhWnd, consoleWindowX, consoleWindowY, CONSOLE_WINDOW_WIDTH, CONSOLE_WINDOW_HEIGHT, TRUE);
	}
	else {
		MoveWindow(mhWnd, 0, 0, CONSOLE_WINDOW_WIDTH, CONSOLE_WINDOW_HEIGHT, TRUE);
	}
	SetConsoleTitle(TEXT("Snake"));
	SetConsoleOutputCP(CP_UTF8);	
}

void ConsoleWindowManager::initLayer()
{
	writeStrPos(0, 0, SYMBOL, DEFAULT_CONSOLE_COLOR);

	int pos = (LEVEL_LEFT + (LEVEL_WIDTH / 2)) - (GAME_NAME.length() / 2);
	writeStrPos(pos, 0, GAME_NAME, DEFAULT_CONSOLE_COLOR);	

	for (int i = 0; i != LEVEL_WIDTH + 1; i++) {
		writeStrPos(LEVEL_LEFT + i, LEVEL_TOP, HORIZONTAL_LEVEL_WALL, DEFAULT_CONSOLE_COLOR);
		writeStrPos(LEVEL_LEFT + i, LEVEL_TOP + LEVEL_HEIGHT, HORIZONTAL_LEVEL_WALL, DEFAULT_CONSOLE_COLOR);
	}
	for (int i = 1; i != LEVEL_HEIGHT; i++) {
		writeStrPos(LEVEL_LEFT, LEVEL_TOP + i, VERTICAL_LEVEL_WALL, DEFAULT_CONSOLE_COLOR);
		writeStrPos(LEVEL_LEFT + LEVEL_WIDTH, LEVEL_TOP + i, VERTICAL_LEVEL_WALL, DEFAULT_CONSOLE_COLOR);
	}	
}

void ConsoleWindowManager::initGame()
{
	initStartPos();	
	gameLoop();
}

void ConsoleWindowManager::initStartPos()
{		
	mSnakeLength = STARTER_SNAKE_LENGTH;
	mSnakeDirection = getRandomNumber(0, 3);
	int startX = getRandomNumber(LEVEL_LEFT + 1, LEVEL_LEFT + LEVEL_WIDTH - 1);
	int startY = getRandomNumber(LEVEL_TOP + 1, LEVEL_TOP + LEVEL_HEIGHT - 1);
	mSnake.push_back(make_tuple(startX, startY));		
}

int ConsoleWindowManager::gameLoop()
{	
	while (true) {
		if (isKeydown(VK_UP))
		{
			mSnakeDirection = 0;
		}
		else if (isKeydown(VK_DOWN)) {
			mSnakeDirection = 1;
		}
		else if (isKeydown(VK_LEFT)) {
			mSnakeDirection = 2;
		}
		else if (isKeydown(VK_RIGHT)) {
			mSnakeDirection = 3;
		}
		else if (isKeydown(VK_ESCAPE)) {
			return 0;
		}
		
		snakeCoord snakeHead = mSnake.at(0);

		switch (mSnakeDirection) {
			case 0: get<1>(snakeHead)--; break;
			case 1: get<1>(snakeHead)++; break;
			case 2: get<0>(snakeHead)--; break;
			case 3: get<0>(snakeHead)++; break;
		}
		mSnake.insert(mSnake.begin(), snakeHead);
		writeSnake();
		if (isWallCollision(snakeHead)) {
			return 0;
		}
		if (mSnake.size() > mSnakeLength) {		//a végének a törlése
			writeStrPos(get<0>(mSnake.at(mSnake.size() - 1)), get<1>(mSnake.at(mSnake.size() - 1)), SNAKE_ELEMENT, 0);
			mSnake.pop_back();
		}
		//this_thread::sleep_for(std::chrono::seconds(1));
		Sleep(50);
	}	
}

bool ConsoleWindowManager::isWallCollision(const snakeCoord& headCoord)
{
	switch (mSnakeDirection) {
		case 0:  
			if (get<1>(headCoord) <= LEVEL_TOP) {
				return true;
			}
			break;
		case 1: 
			if (get<1>(headCoord) >= LEVEL_TOP + LEVEL_HEIGHT) {
				return true;
			}
			break;
		case 2: 
			if (get<0>(headCoord) <= LEVEL_LEFT) {
				return true;
			}
			break;
		case 3: 
			if (get<0>(headCoord) >= LEVEL_LEFT + LEVEL_WIDTH) {
				return true;
			}
			break;
	}
	return false;
}

void ConsoleWindowManager::writeSnake()
{
	if (!mSnake.empty()) {
		bool firstElement = true;
		for each (snakeCoord coord in mSnake)
		{
			if (firstElement) {
				writeStrPos(get<0>(coord), get<1>(coord), SNAKE_ELEMENT, SNAKE_HEAD_COLOR);
				firstElement = false;
			}
			else {
				writeStrPos(get<0>(coord), get<1>(coord), SNAKE_ELEMENT, DEFAULT_CONSOLE_COLOR);
			}
			
		}
	}
}

void ConsoleWindowManager::writeStrPos(const int & x, const int & y, const string & str, const int & color)
{
	mCoord.X = x;
	mCoord.Y = y;
	SetConsoleCursorPosition(mHConsole, mCoord);
	SetConsoleTextAttribute(mHConsole, color);
	cout << str << endl;
}

int ConsoleWindowManager::getRandomNumber(const int & min, const int & max)
{
	//http://en.cppreference.com/w/cpp/numeric/random/random_device
	random_device rd;
	uniform_int_distribution<int> dist(min, max);
	return dist(rd);	
}

bool ConsoleWindowManager::isKeydown(const int & key)
{
	return (GetAsyncKeyState(key) & 0x8000) != 0;
}