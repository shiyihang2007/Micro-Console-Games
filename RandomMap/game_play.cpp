#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <ctime>
#include <queue>
#include <windows.h>
#include "System.hpp"
#include "WinAPI.hpp"
#include "ConDraw.hpp"
using namespace std;
#define MOVE_ERROR 1
int n, m;
int x0, y0, x1, y1;
int ch, f;
int wayCh = ' ';
int difficult_map[] = {10, 30, 50};
int difficult_level = 2;
int fogRegion = 5;
unsigned long long timeCnt = 0;
void cleanCon() {
	SetColor(CON_COLOR(CON_COLOR_DARK_BLACK, CON_COLOR_LIGHT_WHITE));
	for (int i = 0; i < 50; ++i)
		for (int j = 0; j < 50; ++j)
			SetCursor(i, j), putch(' '), putch(' ');
}
class GameMap {
	int mp[1005][1005];
	int discovered[1005][1005];
	int FogOn = 1;
public:
	void SetFog(int fog = -1) {
		FogOn = fog == -1 ? !FogOn : fog;
	}
	void makeMap() {
		cleanCon();
		while (1) {
			SetColor(CON_COLOR(CON_COLOR_DARK_BLACK, CON_COLOR_DARK_WHITE));
			for (int i = 0; i < 10; ++i)
				for (int j = 0; j < 30; ++j)
					SetCursor(i, j), putch(' '), putch(' ');
			SetCursor(1, 1);
			printf("Creating World...");
			char command[55] = "start cmd /c game_map ", tmp[55] = "";
			tmp[0] = difficult_map[difficult_level] / 10 + '0';
			tmp[1] = difficult_map[difficult_level] % 10 + '0';
			strcpy(command + 22, tmp);
			system(command);
			for (int i = 0; i < 10; ++i)
				for (int j = 0; j < 30; ++j)
					SetCursor(i, j), putch(' '), putch(' ');
			SetCursor(1, 1);
			printf("Loading Terrain...");
			Sleep(100);
			freopen("map.txt", "r", stdin);
			int tn = n, tm = m, tx = x1, ty = y1;
			cin >> n >> m >> x0 >> y0 >> x1 >> y1;
			++x0, ++y0, ++x1, ++y1;
			if (n == tn && m == tm && tx == x1 && ty == y1) {
				fclose(stdin);
				continue;
			}
			getchar();
			for (int i = 1; i <= n; ++i) {
				for (int j = 1; j <= m; ++j) {
					ch = getchar();
					if (ch == '#') mp[i][j] = 0;
					else mp[i][j] = 1;
					// Sleep(1);
				}
				getchar();
			}
			mp[x0][y0] = 2, mp[x1][y1] = 3;
			break;
		}
	}
	void clsMap() {
		memset(mp, 0, sizeof(mp));
	}
	void allReload(int n, int m) {
		for (int i = 0; i <= n + 1; ++i)
			for (int j = 0; j <= m + 1; ++j) {
				posReLoad(i, j);
				// cout << mp[i][j];
			}
	}
	void posReLoad(int x, int y, int disableFog = 0) {
		if (x > n + 1 || y > m + 1 || x < 0 || y < 0) return;
		SetCursor(x, y);
		if ((!disableFog && FogOn) && sqrt(pow(abs(x0 - x), 2) + pow(abs(y0 - y), 2)) - fogRegion > -0.1) {
			SetColor(CON_COLOR(CON_COLOR_DARK_WHITE, CON_COLOR_LIGHT_WHITE));
			putch(' '); putch(' ');
			return;
		}
		switch (mp[x][y]) {
		case 0:
			SetColor(CON_COLOR(CON_COLOR_LIGHT_WHITE, CON_COLOR_DARK_BLACK));
			putch(' '); putch(' ');
			break;
		case 1:
			SetColor(CON_COLOR(CON_COLOR_DARK_BLACK, CON_COLOR_LIGHT_WHITE));
			putch(' '); putch(' ');
			break;
		case 2:
			SetColor(CON_COLOR(CON_COLOR_DARK_YELLOW, CON_COLOR_DARK_BLACK));
			putch(' '); putch(' ');
			break;
		case 3:
			SetColor(CON_COLOR(CON_COLOR_LIGHT_RED, CON_COLOR_LIGHT_WHITE));
			putch(' '); putch(' ');
			break;
		case 4:
			SetColor(CON_COLOR(CON_COLOR_DARK_BLACK, CON_COLOR_DARK_WHITE));
			putch(wayCh); putch(wayCh);
			break;
		}
	}
	void regionReLoad(int x, int y) {
		for (int i = -fogRegion - 1; i <= fogRegion; ++i) {
			for (int j = -fogRegion - 1; j <= fogRegion; ++j) {
				posReLoad(x + i, y + j);
			}
		}
	}
	int goUp() {
		if (!mp[x0 - 1][y0]) return 0;
		mp[x0][y0] = 4;
		mp[x0 - 1][y0] = 2;
		--x0;
		if (!FogOn) {
			posReLoad(x0, y0);
			posReLoad(x0 + 1, y0);
		}
		else {
			regionReLoad(x0, y0);
		}
		return 1;
	}
	int goLeft() {
		if (!mp[x0][y0 - 1]) return 0;
		mp[x0][y0] = 4;
		mp[x0][y0 - 1] = 2;
		--y0;
		if (!FogOn) {
			posReLoad(x0, y0);
			posReLoad(x0, y0 + 1);
		}
		else {
			regionReLoad(x0, y0);
		}
		return 1;
	}
	int goRight() {
		if (!mp[x0][y0 + 1]) return 0;
		mp[x0][y0] = 4;
		mp[x0][y0 + 1] = 2;
		++y0;
		if (!FogOn) {
			posReLoad(x0, y0);
			posReLoad(x0, y0 - 1);
		}
		else {
			regionReLoad(x0, y0);
		}
		return 1;
	}
	int goDown() {
		if (!mp[x0 + 1][y0]) return 0;
		mp[x0][y0] = 4;
		mp[x0 + 1][y0] = 2;
		++x0;
		if (!FogOn) {
			posReLoad(x0, y0);
			posReLoad(x0 - 1, y0);
		}
		else {
			regionReLoad(x0, y0);
		}
		return 1;
	}
} gMap;
region Resume, DifficultlySetting;
void ShowDifficultly() {
	freopen("CON", "r", stdin);
	freopen("CON", "w", stdout);
	SetColor(CON_COLOR(CON_COLOR_LIGHT_BLACK, CON_COLOR_LIGHT_WHITE));
	for (int i = 2; i < 20; ++i) {
		for (int j = 10; j < 40; ++j) {
			SetCursor(i, j), putch(' '), putch(' ');
		}
	}
	SetCursor(3, 12);
	printf("Difficult level: %s", (difficult_level == 0 ? "Easy" : difficult_level == 1 ? "Normal" : difficult_level == 2 ? "Hard" : difficult_level == 3 ? "Impossible" : "Error"));
	SetCursor(4, 12);
	printf("New Level(0~3):");
	cin >> difficult_level;
	difficult_level %= 4;
	SetCursor(3, 12);
	printf("Difficult level: %s", (difficult_level == 0 ? "Easy" : difficult_level == 1 ? "Normal" : difficult_level == 2 ? "Hard" : difficult_level == 3 ? "Impossible" : "Error"));
	Sleep(10);
}
int inReg(pos pt) {
	if (CheckCursorOn(Resume, pt))
		return 1;
	if (CheckCursorOn(DifficultlySetting, pt))
		return 2;
	return 0;
}
void DisplaySettings() {
	SetColor(CON_COLOR(CON_COLOR_LIGHT_BLACK, CON_COLOR_LIGHT_WHITE));
	for (int j = 0; j < 9; Sleep(5), ++j)
		for (int i = 0; i < 50; ++i)
			SetCursor(i, j), putch(' '), putch(' ');
	for (int i = 0; i < 50; ++i)
		SetCursor(i, 9), putch(' ');
	SetColor(CON_COLOR(CON_COLOR_DARK_WHITE, CON_COLOR_DARK_BLACK));
	SetCursor(0, 0);
	printf("       MENU        ");
	SetCursor(1, 0);
	DisplayRegion(Resume);
	DisplayRegion(DifficultlySetting);
}
void gameSettings() {
	DisplaySettings();
	while (1) {
		pos point;
		GetCursor(point);
		// debug
		// SetColor(CON_COLOR(CON_COLOR_DARK_BLACK, CON_COLOR_DARK_WHITE));
		// SetCursor(0, 0);
		// printf("%3d%3d", point.x, point.y);
		if (CheckCursorOn(Resume, point)) {
			if (!Resume.HighLight)
				Resume.HighLight = 1, DisplayRegion(Resume);
		}
		else if (Resume.HighLight)
			Resume.HighLight = 0, DisplayRegion(Resume);
		if (CheckCursorOn(DifficultlySetting, point)) {
			if (!DifficultlySetting.HighLight)
				DifficultlySetting.HighLight = 1, DisplayRegion(DifficultlySetting);
		}
		else if (DifficultlySetting.HighLight)
			DifficultlySetting.HighLight = 0, DisplayRegion(DifficultlySetting);
		if (MKEY_DOWN(VK_LBUTTON)) {
			int dbutton = inReg(point);
			if (dbutton == 1) {
				break;
			}
			if (dbutton == 2) {
				ShowDifficultly();
				gMap.makeMap();
				gMap.SetFog(0);
				gMap.allReload(n, m);
				timeCnt = 0;
				break;
			}
			if (dbutton == 3) {
			}
			if (dbutton == 4) {
			}
		}
		Sleep(10);
	}
	SetColor(CON_COLOR(CON_COLOR_LIGHT_BLACK, CON_COLOR_LIGHT_WHITE));
	for (int i = 0; i < 50; ++i)
		SetCursor(i, 9), putch(' ');
	for (int j = 9; j >= 0; --j)
		for (int i = 0; i < 50; ++i)
			SetCursor(i, j), putch(' '), putch(' ');
	for (int j = 9; j >= 0; Sleep(7), --j) {
		for (int i = 0; i <= n + 1; ++i)
			gMap.posReLoad(i, j);
		for (int i = n + 2; i < 50; ++i)
			SetColor(CON_COLOR(CON_COLOR_DARK_BLACK, CON_COLOR_DARK_WHITE)),
			         SetCursor(i, j), putch(' '), putch(' ');
	}
	return;
}
void catchError(int ErrorNum) {
	// freopen("error.log", "w", stdout);
	// if (ErrorNum == MOVE_ERROR) {
	// 	printf("%s<WORN> PLAYER want to Move to the Wall!", ctime(0));
	// 	return;
	// }
	// freopen("CON", "w", stdout);
}
void cls() {
	SetColor(CON_COLOR(CON_COLOR_DARK_BLACK, CON_COLOR_DARK_WHITE));
	HideCursor();
	for (int i = 0; i <= n + 1; ++i)
		for (int j = 0; j <= m + 1; ++j)
			SetCursor(i, j), putch(' '), putch(' ');
}
int main(int argc, char const *argv[])
{
	system("mode con cols=100 lines=50");
	system("title GAMEMAIN");
	hwnd = GetConsoleWindow();
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	HideCursor();
	SetRegion(Resume, 1, 0, 1, 8,
	          CON_COLOR_LIGHT_BLACK, CON_COLOR_LIGHT_WHITE,
	          CON_COLOR_DARK_WHITE, CON_COLOR_DARK_BLACK,
	          "1.RESUME");
	SetRegion(DifficultlySetting, 2, 0, 2, 8,
	          CON_COLOR_LIGHT_BLACK, CON_COLOR_LIGHT_WHITE,
	          CON_COLOR_DARK_WHITE, CON_COLOR_DARK_BLACK,
	          "2.DIFFICULTLY");
	gMap.clsMap();
	gMap.makeMap();
	freopen("CON", "r", stdin);
	freopen("CON", "w", stdout);
	HideCursor();
	SetColor(CON_COLOR(CON_COLOR_DARK_BLACK, CON_COLOR_DARK_WHITE));
	for (int i = 0; i < 50; ++i)
		for (int j = 0; j <= 50; ++j)
			SetCursor(i, j), putch(' '), putch(' ');
	gMap.SetFog();
	gMap.allReload(n, m);
	while (1) {
		if (x0 == x1 && y0 == y1) {
			cls();
			gMap.clsMap();
			SetCursor(1, 1);
			printf("You Win!");
			Sleep(800);
			gMap.makeMap();
			gMap.SetFog();
			gMap.allReload(n, m);
			timeCnt = 0;
		}
		if (difficult_level == 1) {
			if (timeCnt == 200) {
				gMap.SetFog();
				gMap.allReload(n, m);
			}
		}
		else if (difficult_level == 2) {
			if (timeCnt < 250 && timeCnt % 50 == 0) {
				gMap.SetFog();
				gMap.allReload(n, m);
			}
		}
		else if (difficult_level == 3) {
			gMap.SetFog();
			gMap.allReload(n, m);
		}
		if (timeCnt % 500 == 0) {
			gMap.posReLoad(x1, y1, 1);
		}
		if (timeCnt % 500 == 50) {
			gMap.posReLoad(x1, y1, 0);
		}
		while (kbhit()) {
			ch = getch();
			if (f == 1) {
				switch (ch) {
				case KEY_UP:
					if (!gMap.goUp()) {
						catchError(MOVE_ERROR);
					}
					break;
				case KEY_LEFT:
					if (!gMap.goLeft()) {
						catchError(MOVE_ERROR);
					}
					break;
				case KEY_RIGHT:
					if (!gMap.goRight()) {
						catchError(MOVE_ERROR);
					}
					break;
				case KEY_DOWN:
					if (!gMap.goDown()) {
						catchError(MOVE_ERROR);
					}
					break;
				case KEY_INS:
					break;
				case KEY_DEL:
					break;
				case KEY_HOME:
					break;
				case KEY_END:
					break;
				case KEY_PAGEU:
					break;
				case KEY_PAGED:
					break;
				}
				f = 0;
				continue;
			}
			if (f == 2) {
				f = 0;
				continue;
			}
			if (ch == 224) {
				f = 1;
				continue;
			}
			if (ch == 0) {
				f = 2;
				continue;
			}
			switch (ch) {
			case 'w':
				if (!gMap.goUp()) {
					catchError(MOVE_ERROR);
				}
				break;
			case 'a':
				if (!gMap.goLeft()) {
					catchError(MOVE_ERROR);
				}
				break;
			case 'd':
				if (!gMap.goRight()) {
					catchError(MOVE_ERROR);
				}
				break;
			case 's':
				if (!gMap.goDown()) {
					catchError(MOVE_ERROR);
				}
				break;
			case 'r':
				cls();
				gMap.clsMap();
				gMap.makeMap();
				gMap.allReload(n, m);
				break;
			case KEY_ESC:
				gameSettings();
				break;
			}
		}
		// SetCursor(0, 1);
		// printf(" %d %d ", x0, y0);
		++timeCnt;
		Sleep(10);
	}
	return 0;
}
