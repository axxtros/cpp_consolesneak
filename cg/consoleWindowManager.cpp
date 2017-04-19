#include <iostream>

#include "consoleWindowManager.h"

using namespace std;

ConsoleWindowManager::ConsoleWindowManager() {
	//cout << "Object is being created" << endl;
	mHConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	init(mScreenWidth, mScreenHeight);
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
	SetConsoleTitle( TEXT("Snake game v.1.0") );
	
	int x = 10;
	int y = 5;	
	int color = 10;
	string chh = "#";
	writeStrPos(x, y, chh, color);
}

void ConsoleWindowManager::writeStrPos(int & x, int & y, string & str, int & color)
{
	mCoord.X = x;
	mCoord.Y = y;
	SetConsoleCursorPosition(mHConsole, mCoord);
	SetConsoleTextAttribute(mHConsole, color);
	cout << str << endl;
}