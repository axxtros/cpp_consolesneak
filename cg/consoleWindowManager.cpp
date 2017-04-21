//http://en.cppreference.com/w/

#include "consoleWindowManager.h"

using namespace std;

ConsoleWindowManager::ConsoleWindowManager() {	
	mHConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	initConsoleWindow(mScreenWidth, mScreenHeight);
	modes = MENUMOD;
	init(modes);
	//readFile();	
}

void ConsoleWindowManager::readFile()
{
	/*ifstream file("chars.txt");
	string line;
	while (getline(file, line)) {
	cout << line << endl;
	}*/

	setlocale(LC_ALL, "hungary");

	/*wifstream wifs;
	wstring txtline;
	int c = 0;
	wifs.open("chars.txt");
	wifs.imbue(locale(wifs.getloc(), new codecvt_utf8<wchar_t, 0x10ffff, consume_header>()));*/
	/*while (getline(wifs, txtline)) {
		wcout << ++c << L'\t' << txtline << L'\n';
	}*/
	
	//cout << "Hello ф";

	//wcout << endl;
}

ConsoleWindowManager::~ConsoleWindowManager() {
	//cout << "Object is being deleted" << endl;
}

void ConsoleWindowManager::initConsoleWindow(int& mScreenWidth, int& mScreenHeight)
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

void ConsoleWindowManager::init(Modes& modes)
{
	system("cls");
	
	int pos = (LEVEL_LEFT + (LEVEL_WIDTH / 2)) - (GAME_NAME.length() / 2);
	writeStrPos(pos, 0, GAME_NAME, DEFAULT_CONSOLE_COLOR);

	if (modes == MENUMOD) {
		initAndCtrlMainMenu();
	}
	else if (modes == GAMEMOD) {
		initGameLayer();
		initGame();
	}
}

void ConsoleWindowManager::initAndCtrlMainMenu()
{
	int selectedMenu = 1;
	refreshMainMenu(selectedMenu, true);
	while (!isKeydown(VK_RETURN)) {
		if (isKeydown(VK_UP)) {
			selectedMenu--;
			if (selectedMenu == 0)
				selectedMenu = 1;
			refreshMainMenu(selectedMenu, false);
		}			
		if (isKeydown(VK_DOWN)) {
			selectedMenu++;
			if (selectedMenu == 3)
				selectedMenu = 2;
			refreshMainMenu(selectedMenu, false);
		}		
	}
	
	switch (selectedMenu) {
		//new game
		case 1: modes = GAMEMOD; init(modes); break;
		//exit
		case 2: return; break;
		default: return;
	}
}

void ConsoleWindowManager::refreshMainMenu(const int& selectedMenu, bool refreshRecords)
{
	mMenuFirstLine = 10;
	int pos = (LEVEL_LEFT + (LEVEL_WIDTH / 2)) - (MENU_NEW_GAME.length() / 2);
	writeStrPos(pos, mMenuFirstLine, MENU_NEW_GAME, selectedMenu == 1 ? SELECTED_MENU_COLOR : DEFAULT_CONSOLE_COLOR);
	pos = (LEVEL_LEFT + (LEVEL_WIDTH / 2)) - (MENU_EXIT.length() / 2);
	writeStrPos(pos, ++mMenuFirstLine, MENU_EXIT, selectedMenu == 2 ? SELECTED_MENU_COLOR : DEFAULT_CONSOLE_COLOR);
	if (refreshRecords) {
		loadRecords();
	}
}



void ConsoleWindowManager::initGameLayer()
{	
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
	mPlayerPoint = 0;	
	initStartPos();	
	refreshPlayerPoint();
	gameLoop();
}

void ConsoleWindowManager::initStartPos()
{		
	mSnake.clear();
	mSnakeLength = STARTER_SNAKE_LENGTH;
	mSnakeDirection = getRandomNumber(0, 3);
	int startX = getRandomNumber(LEVEL_LEFT + 1, LEVEL_LEFT + LEVEL_WIDTH - 1);
	int startY = getRandomNumber(LEVEL_TOP + 1, LEVEL_TOP + LEVEL_HEIGHT - 1);
	mSnake.push_back(make_tuple(startX, startY));
	mIsExistTarget = false;
}

int ConsoleWindowManager::gameLoop()
{	
	bool runGameLoop = true;
	while (runGameLoop) {
		if (isKeydown(VK_UP))
		{
			if(mSnakeDirection != 1)
				mSnakeDirection = 0;
		}
		else if (isKeydown(VK_DOWN)) {
			if (mSnakeDirection != 0)
				mSnakeDirection = 1;
		}
		else if (isKeydown(VK_LEFT)) {
			if (mSnakeDirection != 3)
				mSnakeDirection = 2;
		}
		else if (isKeydown(VK_RIGHT)) {
			if (mSnakeDirection != 2)
				mSnakeDirection = 3;
		}
		else if (isKeydown(VK_ESCAPE)) {
			return 0;
		}
		
		coord snakeHead = mSnake.at(0);

		switch (mSnakeDirection) {
			case 0: get<1>(snakeHead)--; break;
			case 1: get<1>(snakeHead)++; break;
			case 2: get<0>(snakeHead)--; break;
			case 3: get<0>(snakeHead)++; break;
		}
		mSnake.insert(mSnake.begin(), snakeHead);		
		if (isSnakeCollision(snakeHead)) {
			runGameLoop = false;
		}
		if (isWallCollision(snakeHead)) {
			runGameLoop = false;
		}
		if (!mIsExistTarget || isTargetCollision(snakeHead, mTarget)) {
			generateTarget();
		}
		if (mSnake.size() > mSnakeLength) {		//a végének a törlése
			writeStrPos(get<0>(mSnake.at(mSnake.size() - 1)), get<1>(mSnake.at(mSnake.size() - 1)), SNAKE_ELEMENT, 0);
			mSnake.pop_back();
		}		
		refreshGameArea();
		Sleep(50);
	}	

	saveRecords();
	modes = MENUMOD;
	init(modes);
}

void ConsoleWindowManager::saveRecords()
{
	if (mPlayerPoint != 0) {
		time_t now = time(0);
		//char* dateTime = ctime(&now);
		struct tm * dateTime = localtime(&now);
		ofstream recFile;
		recFile.open(RECORD_FILE, ios::out | ios::app | ios::binary);
		recFile << asctime(dateTime) << to_string(mPlayerPoint) << endl;
		recFile.close();
	}	
}

void ConsoleWindowManager::loadRecords()
{
	struct stat buffer;
	if (stat(RECORD_FILE.c_str(), &buffer) == 0) {
		
		mMenuFirstLine += 4;
		int pos = (LEVEL_LEFT + (LEVEL_WIDTH / 2)) - (RECORDS.length() / 2);
		writeStrPos(pos, mMenuFirstLine, RECORDS, DEFAULT_CONSOLE_COLOR);		
		
		std::ifstream file(RECORD_FILE);
		std::string line;
		int counter = 1;
		string dateStr;
		mRecords.clear();
		while (std::getline(file, line))
		{						
			if (counter % 2 != 0) {
				dateStr = line;
			}
			else {
				Record record;
				record.date = dateStr;
				record.point = line;				
				/*int pos = (LEVEL_LEFT + (LEVEL_WIDTH / 2)) - ((record.date.length() + record.point.length() + 11) / 2);				
				writeStrPos(pos, ++mMenuFirstLine, (record.date + ".........." + record.point), DEFAULT_CONSOLE_COLOR);*/
				mRecords.insert(mRecords.begin(), record);
				counter = 0;
			}			
			counter++;
		}
		
		++mMenuFirstLine;
		sort(mRecords.begin(), mRecords.end());		//ez csak akkor működik, ha van az mRecord-sba operator implmentálva https://www.codeproject.com/Articles/38381/STL-Sort-Comparison-Function

		for (int i = 0; i != ((mRecords.size() - 1) > 10 ? 10 : mRecords.size()); i++) {
			Record record = mRecords[i];
			int pos = (LEVEL_LEFT + (LEVEL_WIDTH / 2)) - ((record.date.length() + record.point.length() + 11) / 2);
			writeStrPos(pos, ++mMenuFirstLine, (record.date + ".........." + record.point), DEFAULT_CONSOLE_COLOR);
		}
	}
}

//int ConsoleWindowManager::compareRecords(const void* first, const void* second)
//{
//	if (first.point < second.point)
//		return -1;
//	else if (first.point == second.point)
//		return 0;
//	else if (first.point > second.point)
//		return 1;
//}

bool ConsoleWindowManager::isSnakeCollision(const coord& snakeHead) {
	for (int i = 1; i != mSnake.size(); i++) {
		coord snakeElementCoord = mSnake.at(i);
		if (get<0>(snakeHead) == get<0>(snakeElementCoord) && get<1>(snakeHead) == get<1>(snakeElementCoord)) {
			return true;
		}
	}
	return false;
}

bool ConsoleWindowManager::isWallCollision(const coord& headCoord)
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

bool ConsoleWindowManager::isTargetCollision(const coord & snakeHead, const coord & target)
{
	if (get<0>(snakeHead) == get<0>(target) && get<1>(snakeHead) == get<1>(target)) {
		targetCollisionEvent();
		mIsExistTarget = false;
		return true;
	}
	else {
		return false;
	}
}

void ConsoleWindowManager::targetCollisionEvent()
{
	mSnakeLength += STARTER_SNAKE_LENGTH;
	mPlayerPoint++;
	refreshPlayerPoint();
}

void ConsoleWindowManager::refreshPlayerPoint() {
	string pointStr = PLAYER_POINT + to_string(mPlayerPoint);
	writeStrPos((LEVEL_LEFT + LEVEL_WIDTH) - (pointStr.length() - 1), LEVEL_TOP - 1, pointStr, DEFAULT_CONSOLE_COLOR);
}

void ConsoleWindowManager::generateTarget()
{
	while (!mIsExistTarget) {
		bool isWrong = false;
		get<0>(mTarget) = getRandomNumber(LEVEL_LEFT + 1, LEVEL_LEFT + LEVEL_WIDTH - 1);
		get<1>(mTarget) = getRandomNumber(LEVEL_TOP + 1, LEVEL_TOP + LEVEL_HEIGHT - 1);
		for each (coord sCoord in mSnake) {
			if (get<0>(mTarget) == get<0>(sCoord) && get<1>(mTarget) == get<1>(sCoord)) {
				isWrong = true;
				break;
			}
		}
		if (!isWrong) {
			mIsExistTarget = true;
		}		
	}
}

void ConsoleWindowManager::refreshGameArea()
{
	if (!mSnake.empty()) {
		bool firstElement = true;
		for each (coord sCoord in mSnake)
		{
			if (firstElement) {
				writeStrPos(get<0>(sCoord), get<1>(sCoord), SNAKE_ELEMENT, SNAKE_HEAD_COLOR);
				firstElement = false;
			}
			else {
				writeStrPos(get<0>(sCoord), get<1>(sCoord), SNAKE_ELEMENT, SNEAK_BODY_COLOR);
			}
			
		}
	}
	if (mIsExistTarget) {
		writeStrPos(get<0>(mTarget), get<1>(mTarget), TARGET_ELEMENT, TARGET_COLOR);
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