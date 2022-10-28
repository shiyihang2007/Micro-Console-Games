#include <winsock2.h>

#include "ConDraw.hpp"
#include "WinAPI.hpp"
#include "System.hpp"

#include <cmath>
#include <ctime>

// #pragma comment(lib, "ws2_32.lib")

using namespace std;

int BackgroundColor = CON_COLOR(CON_COLOR_DARK_BLACK, CON_COLOR_LIGHT_WHITE);
int timeCnt = 0;
int totPlayer = 0;
int maxHealth = 100;

int conDrawBuf[505][505];
int conLstBuf[505][505];
int drawed[505][505];

char mp[250005];
int pos(int x, int y) {
	return x * CON_WIDTH + y;
}

class Player {
public:
	int x, y;
	int color;
	int id;
	int HP;
	int FireDelay[15];
	int protectTick;
	queue<msg > MsgQueue;
	queue<msg > NxtQueue[25];
	Player() : x(1), y(1), color(CON_COLOR(CON_COLOR_LIGHT_WHITE, CON_COLOR_DARK_BLACK)) {
		id = totPlayer++;
		HP = maxHealth;
		protectTick = 10;
	}
	void PlayerMove() {
	}
	void PlayerMoveUp() {
		PlayerMove();
		x = x > 3 ? x - 1 : 3;
		conDrawBuf[x][y] = color;
	}
	void PlayerMoveLeft() {
		PlayerMove();
		y = y > 3 ? y - 1 : 3;
		conDrawBuf[x][y] = color;
	}
	void PlayerMoveRight() {
		PlayerMove();
		y = y < CON_WIDTH - 4 ? y + 1 : CON_WIDTH - 4;
		conDrawBuf[x][y] = color;
	}
	void PlayerMoveDown() {
		PlayerMove();
		x = x < CON_HEIGHT - 4 ? x + 1 : CON_HEIGHT - 4;
		conDrawBuf[x][y] = color;
	}
	void PlayerFire1();
	void PlayerFire2();
	void PlayerFire3();
	void PlayerFire4();
	void PlayerFire5();
	void PlayerFire6();
	void PlayerFire7();
	void Proc(const msg& x) {
		switch (x.type) {
		case MSG_KEY_DOWN_W:
			PlayerMoveUp();
			break;
		case MSG_KEY_DOWN_A:
			PlayerMoveLeft();
			break;
		case MSG_KEY_DOWN_S:
			PlayerMoveDown();
			break;
		case MSG_KEY_DOWN_D:
			PlayerMoveRight();
			break;
		case MSG_KEY_DOWN_SPACE:
			PlayerFire1();
			break;
		case MSG_KEY_DOWN_H:
			PlayerFire2();
			break;
		case MSG_KEY_DOWN_J:
			PlayerFire3();
			break;
		case MSG_KEY_DOWN_K:
			PlayerFire4();
			break;
		case MSG_KEY_DOWN_L:
			PlayerFire5();
			break;
		case MSG_KEY_DOWN_U:
			PlayerFire6();
			break;
		case MSG_KEY_DOWN_I:
			PlayerFire7();
			break;
		}
	}
	void Update();
} PlayerA, PlayerB;

/*Missile*/
class Missile {
public:
	double x, y;
	double px, py;
	Player* fa;
	int liveTime;
	int damage;
	Missile(double a, double b, double c, double d, Player* pa, int atk = 1) : x(a), y(b), px(c), py(d), fa(pa), liveTime(100), damage(atk) {}
	void Track(const Player* en) {
		double tx, ty, pdis, sx, sy, sdis;
		tx = en->x - x;
		ty = en->y - y;
		sx = tx + px, sy = ty + py;
		sdis = sqrt(sx * sx + sy * sy);
		pdis = sqrt(px * px + py * py);
		px = sx * pdis / sdis * 0.95;
		py = sy * pdis / sdis * 0.95;
	}
	void Update() {
		--liveTime;
		if (liveTime == 0) return;
		x += px, y += py;
		if (x < 1) x = 1;
		if (y < 1) y = 1;
		if (x >= CON_HEIGHT - 1) x = CON_HEIGHT - 1;
		if (y >= CON_WIDTH - 1)  y = CON_WIDTH - 1;
		conDrawBuf[(int)x][(int)y] = CON_COLOR(fa->color / 16 - 8, fa->color % 16);
		Player* en = NULL;
		if (fa->id == PlayerA.id) {
			en = &PlayerB;
		}
		else {
			en = &PlayerA;
		}
		Track(en);
	}
};
vector<Missile > missiles;
void Player::PlayerFire1()  {
	if (missiles.size() > 100 || FireDelay[1] > 0) return;
	for (int i = rand() % 10; i < 360; i += rand() % 50 + 10) {
		missiles.push_back(Missile(x, y, sin(i) * 2, cos(i) * 2, this, 2));
	}
	FireDelay[1] = 400;
}
void Player::PlayerFire2()  {
	if (missiles.size() > 100 || FireDelay[2] > 0) return;
	for (int i = rand() % 10; i < 360; i += rand() % 50 + 10) {
		missiles.push_back(Missile(x + sin(i), y + cos(i), sin(i) * 1.5, cos(i) * 1.5, this));
	}
	FireDelay[2] = 160;
}
void Player::PlayerFire3()  {
	if (missiles.size() > 100 || FireDelay[3] > 0) return;
	for (int i = rand() % 50; i < 360; i += rand() % 100 + 10) {
		missiles.push_back(Missile(x, y, sin(i), cos(i), this, 5));
	}
	FireDelay[3] = 100;
}
void Player::PlayerFire4()  {
	if (missiles.size() > 100 || FireDelay[4] > 0) return;
	for (int i = rand() % 50; i < 360; i += rand() % 100 + 10) {
		missiles.push_back(Missile(x, y, sin(i) * 2, cos(i) * 2, this));
	}
	FireDelay[4] = 120;
}
void Player::PlayerFire5()  {
	if (missiles.size() > 100 || FireDelay[5] > 0) return;
	for (int i = rand() % 360; i < 360; i += rand() % 360 + 10) {
		missiles.push_back(Missile(x, y, sin(i) * 2, cos(i) * 2, this));
	}
	FireDelay[5] = 60;
}
/*Laser*/
struct Laser {
public:
	double fx, fy;
	double tx, ty;
	// double px, py;
	Player* fa;
	int damage, liveTime;
	Laser(double x, double y, double ttx, double tty, Player* pa, int atk = 1) : fx(x), fy(y), tx(ttx), ty(tty), fa(pa), damage(atk) {
		liveTime = 100 + rand() % 20;
	}
	void Update() {
		--liveTime;
		double x = fx, y = fy;
		int nowColor = 7;
		if (liveTime > 80 || liveTime < 10) {
			nowColor = (fa->color - 7 * 16);
		}
		else if (liveTime > 0) {
			nowColor = (fa->color + 1 * 16);
		}
		else {
			nowColor = (BackgroundColor);
		}
		Player* en = NULL;
		if (fa->id == PlayerA.id) {
			en = &PlayerB;
		}
		else {
			en = &PlayerA;
		}
		conDrawBuf[(int)x][(int)y] = (fa->color - 6 * 16);
		// SetCursor(1, 1);
		// printf("%2.2lf %2.2lf ", tx, ty);
		int lstx = x, lsty = y;
		while (x > 1 && y > 1 && x < CON_HEIGHT - 1 && y < CON_WIDTH - 1) {
			while ((int)x == lstx && (int)y == lsty) {
				x += tx, y += ty;
			}
			lstx = x, lsty = y;
			conDrawBuf[(int)x][(int)y] = nowColor;
			if (liveTime < 80 && (int)x == en->x && (int)y == en->y && en->protectTick <= 0) {
				en->HP -= damage;
				en->protectTick += 10;
			}
		}
	}
};
vector<Laser > lasers;
void Player::PlayerFire6()  {
	if (lasers.size() > 100 || FireDelay[6] > 0) return;
	Player* en = NULL;
	if (id == PlayerA.id) {
		en = &PlayerB;
	}
	else {
		en = &PlayerA;
	}
	double px = x, py = y;
	double tx = en->x - x, ty = en->y - y;
	tx /= 100, ty /= 100;
	lasers.push_back(Laser(px, py, tx, ty, this, 2));
	FireDelay[6] -= 1;
	NxtQueue[20].push(MSG_KEY_DOWN_U);
	if (FireDelay[6] <= -3) {
		FireDelay[6] = 300;
	}
}
void Player::PlayerFire7()  {
	if (lasers.size() > 100 || FireDelay[7] > 0) return;
	Player* en = NULL;
	if (id == PlayerA.id) {
		en = &PlayerB;
	}
	else {
		en = &PlayerA;
	}
	int angle = rand() % 360;
	double px = x + sin(angle) * 5, py = y + cos(angle) * 5;
	double tx = en->x - px, ty = en->y - py;
	tx /= 100, ty /= 100;
	lasers.push_back(Laser(px, py, tx, ty, this, 1));
	FireDelay[7] -= 1;
	NxtQueue[10].push(MSG_KEY_DOWN_I);
	if (FireDelay[7] <= -5) {
		FireDelay[7] = 300;
	}
}

void Player::Update() {
	for (int i = 0; i < 10; ++i) {
		FireDelay[i] = FireDelay[i] > 0 ? FireDelay[i] - 1 : FireDelay[i];
	}
	protectTick = protectTick > 0 ? protectTick - 1 : 0;
	conDrawBuf[x][y] = color;
	for (int i = 0; i <= 20; ++i) {
		while (!NxtQueue[i].empty()) {
			if (i > 0) {
				NxtQueue[i - 1].push(NxtQueue[i].front());
			}
			else {
				MsgQueue.push(NxtQueue[i].front());
			}
			NxtQueue[i].pop();
		}
	}
	while (!MsgQueue.empty()) {
		Proc(MsgQueue.front());
		MsgQueue.pop();
	}
}

void ProcInput(int ch) {
	switch (ch) {
	case 'W':
		PlayerA.MsgQueue.push(make_msg(MSG_KEY_DOWN_W));
		break;
	case 'A':
		PlayerA.MsgQueue.push(make_msg(MSG_KEY_DOWN_A));
		break;
	case 'S':
		PlayerA.MsgQueue.push(make_msg(MSG_KEY_DOWN_S));
		break;
	case 'D':
		PlayerA.MsgQueue.push(make_msg(MSG_KEY_DOWN_D));
		break;
	case 'H':
		PlayerA.MsgQueue.push(make_msg(MSG_KEY_DOWN_H));
		break;
	case 'J':
		PlayerA.MsgQueue.push(make_msg(MSG_KEY_DOWN_J));
	case 'K':
		PlayerA.MsgQueue.push(make_msg(MSG_KEY_DOWN_K));
		break;
	case 'L':
		PlayerA.MsgQueue.push(make_msg(MSG_KEY_DOWN_L));
		break;
	case 'U':
		if (PlayerA.FireDelay[6] >= 0)
			PlayerA.MsgQueue.push(make_msg(MSG_KEY_DOWN_U));
		break;
	case 'I':
		if (PlayerA.FireDelay[7] >= 0)
			PlayerA.MsgQueue.push(make_msg(MSG_KEY_DOWN_I));
		break;
	case ' ':
		PlayerA.MsgQueue.push(make_msg(MSG_KEY_DOWN_SPACE));
		break;
	case 38:
		PlayerB.MsgQueue.push(make_msg(MSG_KEY_DOWN_W));
		break;
	case 40:
		PlayerB.MsgQueue.push(make_msg(MSG_KEY_DOWN_S));
		break;
	case 37:
		PlayerB.MsgQueue.push(make_msg(MSG_KEY_DOWN_A));
		break;
	case 39:
		PlayerB.MsgQueue.push(make_msg(MSG_KEY_DOWN_D));
		break;
	case '1':
	case 97:
		PlayerB.MsgQueue.push(make_msg(MSG_KEY_DOWN_H));
		break;
	case '2':
	case 98:
		PlayerB.MsgQueue.push(make_msg(MSG_KEY_DOWN_J));
		break;
	case '3':
	case 99:
		PlayerB.MsgQueue.push(make_msg(MSG_KEY_DOWN_K));
		break;
	case '4':
	case 100:
		PlayerB.MsgQueue.push(make_msg(MSG_KEY_DOWN_L));
		break;
	case '5':
	case 101:
		if (PlayerB.FireDelay[6] >= 0)
			PlayerB.MsgQueue.push(make_msg(MSG_KEY_DOWN_U));
		break;
	case '6':
	case 102:
		if (PlayerB.FireDelay[7] >= 0)
			PlayerB.MsgQueue.push(make_msg(MSG_KEY_DOWN_I));
		break;
	case '0':
	case 96:
		PlayerB.MsgQueue.push(make_msg(MSG_KEY_DOWN_SPACE));
		break;
	}
}

int main(int argc, char const *argv[])
{
	/*初始化窗口*/
	srand(time(NULL));
	Sleep(10);
	system("mode con cols=80 lines=40");
	system("mode con rate=40 delay=0");
	system("title Server v0.2");
	hwnd = GetConsoleWindow();
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	HideCursor();
	/*加载网络服务*/
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;
	printf(" <Info> Loading WinSock32.DLL...\n");
	if (WSAStartup(sockVersion, &wsaData) != 0) {
		printf("DLL error !");
		return 0;
	}
	Sleep(10);
	printf(" <Info> Creating Socket...\n");
	SOCKET slisten = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (slisten == INVALID_SOCKET) {
		printf("socket error !");
		return 0;
	}
	Sleep(10);
	printf(" <Info> Setting Ports...\n");
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(25565);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	Sleep(10);
	printf(" <Info> Bind Socket with Ports...\n");
	if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR) {
		printf("bind error !");
	}
	Sleep(10);
	printf(" <Info> Checking Socket...\n");
	if (listen(slisten, 5) == SOCKET_ERROR)
	{
		printf("listen error !");
		return 0;
	}
	Sleep(10);
	printf(" <Info> Preparing Client Cache...\n");
	SOCKET sClient;
	sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);
	char revData[255];
	Sleep(10);
	printf(" <Info> Network Init.\n");
	/*游戏初始化*/
	// HideCursor();
	Sleep(10);
	printf(" <Info> Loading Game Data...\n");
	PlayerA.color = CON_COLOR(CON_COLOR_LIGHT_BLUE, CON_COLOR_DARK_BLACK);
	PlayerB.color = CON_COLOR(CON_COLOR_LIGHT_RED, CON_COLOR_DARK_BLACK);
	PlayerA.x = 4, PlayerA.y = 4;
	PlayerB.x = CON_HEIGHT - 5, PlayerB.y = CON_WIDTH - 5;
	Sleep(10);
	printf(" <Info> Game Init.\n");
	SetCursor(CON_HEIGHT - 3, 0);
	printf(" <State> Offline.");
	while (PlayerA.HP > 0 && PlayerB.HP > 0) {
		SetColor(CON_COLOR(CON_COLOR_DARK_BLACK, CON_COLOR_DARK_WHITE));
		SetCursor(CON_HEIGHT - 2, 0);
		printf(" <Info> TimeCnt=%3d", timeCnt);
		int nowColor = BackgroundColor;
		for (int i = 0; i < (int)missiles.size(); ++i) {
			missiles[i].Update();
			if (int(missiles[i].x) == PlayerA.x && int(missiles[i].y) == PlayerA.y && missiles[i].fa->id != PlayerA.id) {
				PlayerA.HP -= missiles[i].damage;
				missiles.erase(missiles.begin() + i);
				--i;
				continue;
			}
			else if (int(missiles[i].x) == PlayerB.x && int(missiles[i].y) == PlayerB.y && missiles[i].fa->id != PlayerB.id) {
				PlayerB.HP -= missiles[i].damage;
				missiles.erase(missiles.begin() + i);
				--i;
				continue;
			}
			else if (missiles[i].x <= 1 || missiles[i].y <= 1 || missiles[i].x >= CON_HEIGHT - 2 || missiles[i].y >= CON_WIDTH - 2) {
				missiles.erase(missiles.begin() + i);
				--i;
				continue;
			}
			if (missiles[i].liveTime == 0) {
				missiles.erase(missiles.begin() + i);
				--i;
				continue;
			}
		}
		for (int i = 0; i < (int)lasers.size(); ++i) {
			lasers[i].Update();
			if (lasers[i].liveTime <= 0) {
				lasers.erase(lasers.begin() + i);
				--i;
				continue;
			}
		}
		PlayerA.Update();
		PlayerB.Update();
		// SetCursor(1, 1);
		// printf("%d", timeCnt);
		// SetColor(PlayerA.color);
		// SetCursor(0, 1);
		// printf("%3d", PlayerA.HP);
		// SetColor(PlayerB.color);
		// SetCursor(0, 5);
		// printf("%3d", PlayerB.HP);
		/*debug*/
		// SetColor(PlayerA.color);
		// SetCursor(1, 1);
		// printf("%3d", PlayerA.HP * CON_HEIGHT / maxHealth);
		// SetColor(PlayerB.color);
		// SetCursor(1, 5);
		// printf("%3d", PlayerB.HP * CON_HEIGHT / maxHealth);
		/*Health*/
		nowColor = ((PlayerA.color / 16 - 8) * 16);
		for (int i = 0; i < CON_HEIGHT; ++i) {
			if (i == CON_HEIGHT - PlayerA.HP * CON_HEIGHT / maxHealth) {
				nowColor = (PlayerA.color);
			}
			conDrawBuf[i][0] = nowColor;
		}
		nowColor = ((PlayerB.color / 16 - 8) * 16);
		for (int i = 0; i < CON_HEIGHT; ++i) {
			if (i == CON_HEIGHT - PlayerB.HP * CON_HEIGHT / maxHealth) {
				nowColor = (PlayerB.color);
			}
			conDrawBuf[i][CON_WIDTH - 1] = nowColor;
		}
		int lst = 0;
		for (int i = 1; i < 10; ++i) {
			// SetCursor(i + 10, 1);
			if (PlayerA.FireDelay[i] > 0) lst = ((PlayerA.color / 16 - 8) * 16 + 7);
			else lst = (PlayerA.color);
			// printf("%-4d", PlayerA.FireDelay[i]);
			mp[pos(i + 10, 1)] = lst / 16;
			mp[pos(i + 10, 2)] = lst / 16;
			drawed[i + 10][1] = 1;
			drawed[i + 10][2] = 1;
		}
		for (int i = 1; i < 10; ++i) {
			// SetCursor(i + 10, CON_WIDTH - 3);
			if (PlayerB.FireDelay[i] > 0) lst = ((PlayerB.color / 16 - 8) * 16 + 7);
			else lst = (PlayerB.color);
			// printf("%4d", PlayerB.FireDelay[i]);
			mp[pos(i + 10, CON_WIDTH - 3)] = lst / 16;
			mp[pos(i + 10, CON_WIDTH - 2)] = lst / 16;
			drawed[i + 10][CON_WIDTH - 3] = 1;
			drawed[i + 10][CON_WIDTH - 2] = 1;
		}
		++timeCnt;
		lst = conDrawBuf[0][0];
		for (int i = 0; i < CON_HEIGHT; ++i) {
			for (int j = 0; j < CON_WIDTH; ++j) {
				if (drawed[i][j]) continue;
				if (conDrawBuf[i][j] == conLstBuf[i][j]) continue;
				if (conDrawBuf[i][j] != lst) {
					lst = conDrawBuf[i][j];
				}
				mp[pos(i, j)] = lst / 16;
			}
		}
		/*DEBUG*/
		// for (int i = 0; i < 1600; ++i) {
		// 	SetCursor(i / 40, i % 40);
		// 	SetColor(mp[i] * 16);
		// 	putch(' '), putch(' ');
		// }
		/*向客户端请求数据*/
		// printf("Waiting connect...\n");
		sClient = accept(slisten, (SOCKADDR *)&remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET)
		{
			printf("Accept error !");
			continue;
		}
		// printf("Connect with: %s \r\n", inet_ntoa(remoteAddr.sin_addr));
		//接收数据
		int ret = recv(sClient, revData, 255, 0);
		SetColor(CON_COLOR(CON_COLOR_DARK_BLACK, CON_COLOR_DARK_WHITE));
		SetCursor(CON_HEIGHT - 3, 0);
		printf(" <State> Online. DATA: [ ]");
		if (ret > 0) {
			revData[ret] = 0x00;
			// for (int i = 0; i < ret; ++i) {
			// 	printf("%d ", revData[i]);
			// }
			// printf("\n");
			int cnt = 0, ch = 0;
			while (cnt < ret) {
				ch = revData[cnt++];
				ProcInput(ch);
				SetCursor(CON_HEIGHT - 3, 12);
				putch(ch);
			}
		}
		/*发送画面*/
		send(sClient, mp, 1600, 0);
		closesocket(sClient);
		memcpy(conLstBuf, conDrawBuf, sizeof(conLstBuf));
		memset(conDrawBuf, 0, sizeof(conDrawBuf));
		memset(drawed, 0, sizeof(drawed));
		Sleep(0);
	}
	while (kbhit()) getch();
	system("color 07 & cls");
	SetCursor(1, 1);
	if (PlayerA.HP > 0) {
		printf("PlayerA Win\n");
	}
	else {
		printf("PlayerB Win\n");
	}
	Sleep(1000);
	return 0;
}