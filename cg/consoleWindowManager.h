#pragma once
#ifndef CONSOLEMANAGER_H
#define CONSOLEMANAGER_H

#include <windows.h>
#include <string>
#include <tchar.h>

using namespace std;

class ConsoleWindowManager {	

	static const bool IS_CENTER_SCREEN = true;
	static const int CONSOLE_WINDOW_WIDTH = 1000;
	static const int CONSOLE_WINDOW_HEIGHT = 600;
	static const int DEFAULT_CONSOLE_COLOR = 10;

private:	
	HANDLE mHConsole;
	HWND mhWnd;
	RECT mRect;
	COORD mCoord;
	int mScreenWidth;
	int mScreenHeight;

public:
	ConsoleWindowManager();
	~ConsoleWindowManager();
	void init(int&, int&);
	void writeStrPos(int& x, int& y, string & ch, int & color);
};

#endif // ! CONSOLEMANAGER_H

