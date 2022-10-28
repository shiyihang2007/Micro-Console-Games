#include "ConDraw.hpp"
#include "WinAPI.hpp"
#include "System.hpp"

#include <cmath>
#include <ctime>

using namespace std;

int BackgroundColor = CON_COLOR(CON_COLOR_DARK_BLACK, CON_COLOR_LIGHT_WHITE);
int timeCnt = 0;
int totPlayer = 0;
int maxHealth = 500;

int conDrawBuf[505][505];
int conLstBuf[505][505];
int drawed[505][505];

class Player {
public:
	int x, y;
	int color;
	int id;
	double HP;
	double lstHP;
	int goOn;
	int FireDelay[15];
	int protectTick;
	queue<msg > MsgQueue;
	queue<msg > NxtQueue[105];
	Player() : x(1), y(1), color(CON_COLOR(CON_COLOR_LIGHT_WHITE, CON_COLOR_DARK_BLACK)) {
		id = totPlayer++;
		HP = maxHealth;
		protectTick = 10;
	}
	void PlayerMove() {
	}
	void PlayerMoveUp() {
		PlayerMove();
		x = x > 2 ? x - 1 : 2;
		conDrawBuf[x][y] = color;
	}
	void PlayerMoveLeft() {
		PlayerMove();
		y = y > 2 ? y - 1 : 2;
		conDrawBuf[x][y] = color;
	}
	void PlayerMoveRight() {
		PlayerMove();
		y = y < CON_WIDTH - 3 ? y + 1 : CON_WIDTH - 3;
		conDrawBuf[x][y] = color;
	}
	void PlayerMoveDown() {
		PlayerMove();
		x = x < CON_HEIGHT - 3 ? x + 1 : CON_HEIGHT - 3;
		conDrawBuf[x][y] = color;
	}
	void PlayerFire1();
	void PlayerFire2();
	void PlayerFire3();
	void PlayerFire4();
	void PlayerFire5();
	void PlayerFire6();
	void PlayerFire7();
	void Proc(const msg& msgx) {
		switch (msgx.type) {
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
} PlayerA, AI;

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
			en = &AI;
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
	FireDelay[1] = 800;
}
void Player::PlayerFire2()  {
	if (missiles.size() > 100 || FireDelay[2] > 0) return;
	for (int i = rand() % 10; i < 360; i += rand() % 50 + 10) {
		missiles.push_back(Missile(x + sin(i), y + cos(i), sin(i) * 1.5, cos(i) * 1.5, this));
	}
	FireDelay[2] = 320;
}
void Player::PlayerFire3()  {
	if (missiles.size() > 100 || FireDelay[3] > 0) return;
	for (int i = rand() % 50; i < 360; i += rand() % 100 + 10) {
		missiles.push_back(Missile(x, y, sin(i), cos(i), this, 5));
	}
	FireDelay[3] = 200;
}
void Player::PlayerFire4()  {
	if (missiles.size() > 100 || FireDelay[4] > 0) return;
	for (int i = rand() % 50; i < 360; i += rand() % 100 + 10) {
		missiles.push_back(Missile(x, y, sin(i) * 2, cos(i) * 2, this));
	}
	FireDelay[4] = 240;
}
void Player::PlayerFire5()  {
	if (missiles.size() > 100 || FireDelay[5] > 0) return;
	for (int i = rand() % 360; i < 360; i += rand() % 360 + 10) {
		missiles.push_back(Missile(x, y, sin(i) * 2, cos(i) * 2, this));
	}
	FireDelay[5] = 120;
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
		int nowColor = (fa->color - 7 * 16);
		if (liveTime > 80 || liveTime < 10) {
			nowColor = (fa->color - 7 * 16);
		}
		else if (liveTime >= 10) {
			nowColor = (fa->color + 1 * 16);
		}
		Player* en = NULL;
		if (fa->id == PlayerA.id) {
			en = &AI;
		}
		else {
			en = &PlayerA;
		}
		conDrawBuf[(int)x][(int)y] = (fa->color - 6 * 16);
		int lstx = (int)x, lsty = (int)y;
		while (x > 1 && y > 1 && x < CON_HEIGHT - 1 && y < CON_WIDTH - 1) {
			while ((int)x == lstx && (int)y == lsty) {
				x += tx, y += ty;
			}
			lstx = (int)x, lsty = (int)y;
			conDrawBuf[(int)x][(int)y] = nowColor;
			if (liveTime > 10 && liveTime < 80 && (int)x == en->x && (int)y == en->y && en->protectTick <= 0) {
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
		en = &AI;
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
		FireDelay[6] = 600;
	}
}
void Player::PlayerFire7()  {
	if (lasers.size() > 100 || FireDelay[7] > 0) return;
	Player* en = NULL;
	if (id == PlayerA.id) {
		en = &AI;
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
		FireDelay[7] = 600;
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

int dx[] = {0, 0, 1, 1, 1, -1, -1, -1};
int dy[] = {1, -1, 1, -1, 0, 1, -1, 0};
vector<msg > actives;
int vis[505][505];
int flag;
FILE* flog;

int willHurt(int x, int y, Player* en) {
	return (conDrawBuf[x][y] / 16 == en->color / 16 ||
	        conDrawBuf[x][y] / 16 == en->color / 16 - 8 ||
	        conDrawBuf[x][y] / 16 == en->color / 16 + 1 ||
	        conDrawBuf[x][y] / 16 == en->color / 16 - 7 ||
	        conDrawBuf[x][y] / 16 == en->color / 16 + 2 ||
	        conDrawBuf[x][y] / 16 == en->color / 16 - 6);
}
int aiWarning(int x, int y, int dis, Player* en) {
	for (int i = x - dis; i < x + dis; ++i) {
		if (i > CON_HEIGHT - 3 || i < 2) continue;
		for (int j = y - dis; j < y + dis; ++j) {
			if (j > CON_HEIGHT - 3 || j < 2) continue;
			if (willHurt(i, j, en)) {
				return 1;
			}
		}
	}
	return 0;
}
void dodgeAIdfs(int x, int y, int d, Player &Ai, Player &Player) {
	if (actives.size() > 10) {
		return;
	}
	if (!(willHurt(x - 1, y, &Player) || willHurt(x + 1, y, &Player) ||
	        willHurt(x, y - 1, &Player) || willHurt(x, y + 1, &Player) || willHurt(x, y, &Player)) &&
	        x != Ai.x && y != Ai.y && !(x == Player.x && y == Player.y)) {
		for (int i = 0; i < (int)actives.size(); ++i) {
			Ai.NxtQueue[i].push(actives[i]);
		}
		actives.clear();
		flag = 1;
		return;
	}
	for (int i = 0; i < 8 && !flag; ++i) {
		int tx = x + dx[i], ty = y + dy[i];
		if (tx > 2 && tx < CON_HEIGHT - 2 && ty > 2 && ty < CON_WIDTH - 2 &&
		        !vis[tx][ty]) {
			vis[tx][ty] = 1;
			if (dx[i]) {
				actives.push_back(make_msg((dx[i] == 1 ? MSG_KEY_DOWN_S : MSG_KEY_DOWN_W)));
			}
			if (dy[i]) {
				actives.push_back(make_msg((dy[i] == 1 ? MSG_KEY_DOWN_D : MSG_KEY_DOWN_A)));
			}
			dodgeAIdfs(tx, ty, d + 1, Ai, Player);
			if (dx[i]) {
				actives.pop_back();
			}
			if (dy[i]) {
				actives.pop_back();
			}
			vis[tx][ty] = 0;
		}
	}
}
void dodgeAIbfs(int x, int y, Player &Ai, Player &Player) {
	priority_queue<pair<int, pair<int, int> >, vector<pair<int, pair<int, int> > >, greater<pair<int, pair<int, int> >> > q;
	pair<int, int > path[CON_HEIGHT][CON_WIDTH] = {};
	fprintf(flog, "<info> AI dodge from %d %d\n", x, y);
	q.push(make_pair(10, make_pair(x, y)));
	int endx = x, endy = y;
	path[x][y] = make_pair(-1, -1);
	vis[x][y] = 1;
	while (!q.empty()) {
		pair<int, int > now = q.top().second;
		int step = q.top().first;
		q.pop();
		if (!aiWarning(now.first, now.second, 5, &Player) &&
		        now.first != Ai.x && now.second != Ai.y &&
		        !(now.first == Player.x && now.second == Player.y)) {
			endx = now.first, endy = now.second;
			break;
		}
		for (int i = 0; i < 8; ++i) {
			int tx = now.first + dx[i], ty = now.second + dy[i];
			// fprintf(flog, "<info> try %d %d\n", tx, ty);
			if (tx > CON_HEIGHT - 3 || ty > CON_WIDTH - 3 || tx < 2 || ty < 2 || vis[tx][ty]) {
				continue;
			}
			int v = step + 1;
			if (willHurt(tx, ty, &Player)) {
				v += 300;
			}
			if (willHurt(tx + 1, ty, &Player) ||
				willHurt(tx - 1, ty, &Player) ||
				willHurt(tx, ty + 1, &Player) ||
				willHurt(tx, ty - 1, &Player)) {
				v += 100;
			}
			if (aiWarning(now.first, now.second, 3, &Player)) {
				v += 80;
			}
			if (aiWarning(now.first, now.second, 8, &Player)) {
				v += 50;
			}
			if (aiWarning(now.first, now.second, 15, &Player)) {
				v += 30;
			}
			if (aiWarning(now.first, now.second, 20, &Player)) {
				v += 10;
			}
			v += rand() % 10;
			q.push(make_pair(v, make_pair(tx, ty)));
			path[tx][ty] = make_pair(now.first, now.second);
			vis[tx][ty] = 1;
		}
	}
	fprintf(flog, "<info> AI dodge to %d %d AI.goOn=%d\n", endx, endy, Ai.goOn);
	while (path[endx][endy].first >= 0) {
		pair<int, int > tmp = path[endx][endy];
		if (abs(tmp.first - endx) > 0) {
			actives.push_back(make_msg((path[endx][endy].first - endx == -1 ? MSG_KEY_DOWN_S : MSG_KEY_DOWN_W)));
			// fprintf(flog, "<info> go %s\n", (path[endx][endy].first - endx == -1 ? "Down" : "Up"));
		}
		if (abs(tmp.second - endy) > 0) {
			actives.push_back(make_msg((path[endx][endy].second - endy == -1 ? MSG_KEY_DOWN_D : MSG_KEY_DOWN_A)));
			// fprintf(flog, "<info> go %s\n", (path[endx][endy].second - endy == -1 ? "Right" : "Left"));
		}
		// fprintf(flog, "<info> goto %d %d\n", endx, endy);
		endx = tmp.first, endy = tmp.second;
	}
	int count = 0;
	while (!actives.empty()) {
		Ai.NxtQueue[count++ / 4].push(*(actives.end() - 1));
		actives.pop_back();
	}
	Ai.goOn = count / 4 + 1;
	fprintf(flog, "<info> AI dodge count=%d AI.goOn=%d\n", count, Ai.goOn);
}
void approachAI(int x, int y, double dis, Player &Ai, Player &Player) {
	if (Ai.goOn) return;
	int tot = 0;
	while (sqrt(max(pow(x - Player.x, 2) + pow(y - Player.y, 2), 1.0)) > dis && tot < 20) {
		if (x > Player.x + 2) {
			--x, Ai.NxtQueue[tot].push(MSG_KEY_DOWN_W);
		}
		if (x < Player.x - 2) {
			++x, Ai.NxtQueue[tot].push(MSG_KEY_DOWN_S);
		}
		if (y > Player.y + 2) {
			--y, Ai.NxtQueue[tot].push(MSG_KEY_DOWN_A);
		}
		if (y < Player.y - 2) {
			++y, Ai.NxtQueue[tot].push(MSG_KEY_DOWN_D);
		}
		++tot;
	}
}
void approachAI(int x, int y, int tx, int ty, double dis, Player &Ai) {
	if (Ai.goOn) return;
	int tot = 0;
	while (sqrt(max(pow(x - tx, 2) + pow(y - ty, 2), 1.0)) > dis && tot < 20) {
		if (x > tx + 2) {
			--x, Ai.NxtQueue[tot].push(MSG_KEY_DOWN_W);
		}
		if (x < tx - 2) {
			++x, Ai.NxtQueue[tot].push(MSG_KEY_DOWN_S);
		}
		if (y > ty + 2) {
			--y, Ai.NxtQueue[tot].push(MSG_KEY_DOWN_A);
		}
		if (y < ty - 2) {
			++y, Ai.NxtQueue[tot].push(MSG_KEY_DOWN_D);
		}
		++tot;
	}
}
void backAI(int x, int y, double dis, Player &Ai, Player &Player) {
	if (Ai.goOn) return;
	int tot = 0;
	while (sqrt(max(pow(x - Player.x, 2) + pow(y - Player.y, 2), 0.1)) < dis && tot < 20) {
		if (x > Player.x + 1 && x < CON_HEIGHT - 3) {
			++x, Ai.NxtQueue[tot].push(MSG_KEY_DOWN_W);
		}
		if (x < Player.x - 1 && x > 2) {
			--x, Ai.NxtQueue[tot].push(MSG_KEY_DOWN_S);
		}
		if (y > Player.y + 1 && y < CON_WIDTH - 3) {
			++y, Ai.NxtQueue[tot].push(MSG_KEY_DOWN_A);
		}
		if (y < Player.y - 1 && y > 2) {
			--y, Ai.NxtQueue[tot].push(MSG_KEY_DOWN_D);
		}
		++tot;
	}
}
void AIActive(Player &Ai, Player &Player) {
	Ai.goOn = Ai.goOn > 0 ? Ai.goOn - 1 : 0;
	if ((Ai.lstHP > Ai.HP || aiWarning(Ai.x, Ai.y, 8, &Player) ||
	        (Ai.x == Player.x && Ai.y == Player.y)) && Ai.goOn == 0) {
		while(!Ai.MsgQueue.empty()) Ai.MsgQueue.pop();
		flag = 0;
		memset(vis, 0, sizeof(vis));
		dodgeAIbfs(Ai.x, Ai.y, Ai, Player);
		Ai.lstHP = Ai.HP;
	}
	if (sqrt(max(pow(Ai.x - Player.x, 2) + pow(Ai.y - Player.y, 2), 0.1)) > 5) {
		if (Ai.FireDelay[6] == 0)
			Ai.MsgQueue.push(make_msg(MSG_KEY_DOWN_U));
		if (Ai.FireDelay[7] == 0)
			Ai.MsgQueue.push(make_msg(MSG_KEY_DOWN_I));
	}
	if (sqrt(max(pow(Ai.x - Player.x, 2) + pow(Ai.y - Player.y, 2), 0.1)) < 30) {
		if (Ai.FireDelay[1] == 0)
			Ai.MsgQueue.push(make_msg(MSG_KEY_DOWN_SPACE));
		if (Ai.FireDelay[4] == 0)
			Ai.MsgQueue.push(make_msg(MSG_KEY_DOWN_K));
		if (Ai.FireDelay[5] == 0)
			Ai.MsgQueue.push(make_msg(MSG_KEY_DOWN_L));
	}
	if (sqrt(max(pow(Ai.x - Player.x, 2) + pow(Ai.y - Player.y, 2), 0.1)) < 20) {
		if (Ai.FireDelay[2] == 0)
			Ai.MsgQueue.push(make_msg(MSG_KEY_DOWN_H));
		if (Ai.FireDelay[3] == 0)
			Ai.MsgQueue.push(make_msg(MSG_KEY_DOWN_J));
	}
	if (sqrt(max(pow(Ai.x - Player.x, 2) + pow(Ai.y - Player.y, 2), 0.1)) > 15 &&
	        (Ai.FireDelay[6] < 10 || Ai.FireDelay[7] < 10) && Ai.goOn == 0) {
		approachAI(Ai.x, Ai.y, 15, Ai, Player);
		Ai.goOn = 5;
	}
	if (sqrt(max(pow(Ai.x - Player.x, 2) + pow(Ai.y - Player.y, 2), 0.1)) > 20 &&
	        (Ai.FireDelay[4] < 10 || Ai.FireDelay[5] < 10) && Ai.goOn == 0) {
		approachAI(Ai.x, Ai.y, 20, Ai, Player);
		Ai.goOn = 5;
	}
	if (sqrt(max(pow(Ai.x - Player.x, 2) + pow(Ai.y - Player.y, 2), 0.1)) > 15 &&
	        (Ai.FireDelay[2] < 10 || Ai.FireDelay[3] < 10) && Ai.goOn == 0) {
		approachAI(Ai.x, Ai.y, 15, Ai, Player);
		Ai.goOn = 5;
	}
	if (sqrt(max(pow(Ai.x - CON_HEIGHT / 2, 2) + pow(Ai.y - CON_WIDTH / 2, 2), 0.1)) > 20 && Ai.goOn == 0) {
		approachAI(Ai.x, Ai.y, CON_HEIGHT / 2, CON_WIDTH / 2, 5, Ai);
		Ai.goOn = 8;
	}
	if (sqrt(max(pow(Ai.x - Player.x, 2) + pow(Ai.y - Player.y, 2), 0.1)) < 15 &&
	        (Ai.FireDelay[2] > 10 && Ai.FireDelay[3] > 10) && Ai.goOn == 0) {
		backAI(Ai.x, Ai.y, 30, Ai, Player);
		Ai.goOn = 5;
	}
	if (sqrt(max(pow(Ai.x - Player.x, 2) + pow(Ai.y - Player.y, 2), 0.1)) < 15 && Ai.goOn == 0) {
		backAI(Ai.x, Ai.y, 20, Ai, Player);
		Ai.goOn = 2;
	}
	if (sqrt(max(pow(Ai.x - Player.x, 2) + pow(Ai.y - Player.y, 2), 0.1)) > 30 && Ai.goOn == 0) {
		approachAI(Ai.x, Ai.y, 30, Ai, Player);
		Ai.goOn = 2;
	}
	Ai.lstHP = Ai.HP;
}
void printCh(int ch) {
	if (ch == '3') {
		char s[6][6] = {
			" ### ",
			"#   #",
			"   # ",
			"#   #",
			" ### "
		};
		for (int i = 0; i < 5; ++i) {
			for (int j = 0; j < 5; ++j) {
				SetCursor(CON_HEIGHT / 2 - 2 + i, CON_WIDTH / 2 - 2 + j);
				if (s[i][j] == ' ') {
					SetColor(BackgroundColor);
				}
				else {
					SetColor(CON_COLOR(CON_COLOR_LIGHT_WHITE, CON_COLOR_DARK_BLACK));
				}
				putch(' '), putch(' ');
			}
		}
	}
	else if (ch == '2') {
		char s[6][6] = {
			" ### ",
			"#   #",
			"  ## ",
			" #   ",
			"#####"
		};
		for (int i = 0; i < 5; ++i) {
			for (int j = 0; j < 5; ++j) {
				SetCursor(CON_HEIGHT / 2 - 2 + i, CON_WIDTH / 2 - 2 + j);
				if (s[i][j] == ' ') {
					SetColor(BackgroundColor);
				}
				else {
					SetColor(CON_COLOR(CON_COLOR_LIGHT_WHITE, CON_COLOR_DARK_BLACK));
				}
				putch(' '), putch(' ');
			}
		}
	}
	else if (ch == '1') {
		char s[6][6] = {
			"  #  ",
			"  ## ",
			"  #  ",
			"  #  ",
			"#####"
		};
		for (int i = 0; i < 5; ++i) {
			for (int j = 0; j < 5; ++j) {
				SetCursor(CON_HEIGHT / 2 - 2 + i, CON_WIDTH / 2 - 2 + j);
				if (s[i][j] == ' ') {
					SetColor(BackgroundColor);
				}
				else {
					SetColor(CON_COLOR(CON_COLOR_LIGHT_WHITE, CON_COLOR_DARK_BLACK));
				}
				putch(' '), putch(' ');
			}
		}
	}
}

int main(int argc, char const *argv[])
{
	if (argc > 0) srand(argv[0][0]);
	else srand((unsigned)time(NULL));
	Sleep(10);
	system("mode con cols=80 lines=40");
	system("mode con rate=40 delay=0");
	system("title 2DPvP");
	hwnd = GetConsoleWindow();
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	HideCursor();
	flog = fopen("game.log", "w+");
	PlayerA.color = CON_COLOR(CON_COLOR_LIGHT_BLUE, CON_COLOR_DARK_BLACK);
	AI.color = CON_COLOR(CON_COLOR_LIGHT_RED, CON_COLOR_DARK_BLACK);
	PlayerA.x = 2, PlayerA.y = 2;
	AI.x = CON_HEIGHT - 3, AI.y = CON_WIDTH - 3;
	PlayerA.Update(), AI.Update();
	// printCh('3'), Sleep(1000);
	// printCh('2'), Sleep(1000);
	// printCh('1'), Sleep(1000);
	while (PlayerA.HP > 0 && AI.HP > 0) {
		int nowColor = BackgroundColor;
		if (MKEY_DOWN('W')) {
			PlayerA.MsgQueue.push(make_msg(MSG_KEY_DOWN_W));
		}
		if (MKEY_DOWN('A')) {
			PlayerA.MsgQueue.push(make_msg(MSG_KEY_DOWN_A));
		}
		if (MKEY_DOWN('S')) {
			PlayerA.MsgQueue.push(make_msg(MSG_KEY_DOWN_S));
		}
		if (MKEY_DOWN('D')) {
			PlayerA.MsgQueue.push(make_msg(MSG_KEY_DOWN_D));
		}
		if (MKEY_DOWN(' ')) {
			PlayerA.MsgQueue.push(make_msg(MSG_KEY_DOWN_SPACE));
		}
		if (MKEY_DOWN('H')) {
			PlayerA.MsgQueue.push(make_msg(MSG_KEY_DOWN_H));
		}
		if (MKEY_DOWN('J')) {
			PlayerA.MsgQueue.push(make_msg(MSG_KEY_DOWN_J));
		}
		if (MKEY_DOWN('K')) {
			PlayerA.MsgQueue.push(make_msg(MSG_KEY_DOWN_K));
		}
		if (MKEY_DOWN('L')) {
			PlayerA.MsgQueue.push(make_msg(MSG_KEY_DOWN_L));
		}
		if (MKEY_DOWN('U')) {
			PlayerA.MsgQueue.push(make_msg(MSG_KEY_DOWN_U));
		}
		if (MKEY_DOWN('I')) {
			PlayerA.MsgQueue.push(make_msg(MSG_KEY_DOWN_I));
		}
		/*
		while (kbhit()) {
			int ch = getch();
			if (ch == 224) {
				ch = getch();
				switch (ch) {
				}
			}
			else {
				switch (ch) {
				case 'w':
				case 'W':
					PlayerA.MsgQueue.push(make_msg(MSG_KEY_DOWN_W));
					break;
				case 'a':
				case 'A':
					PlayerA.MsgQueue.push(make_msg(MSG_KEY_DOWN_A));
					break;
				case 's':
				case 'S':
					PlayerA.MsgQueue.push(make_msg(MSG_KEY_DOWN_S));
					break;
				case 'd':
				case 'D':
					PlayerA.MsgQueue.push(make_msg(MSG_KEY_DOWN_D));
					break;
				case 'H':
				case 'h':
					PlayerA.MsgQueue.push(make_msg(MSG_KEY_DOWN_H));
					break;
				case 'J':
				case 'j':
					PlayerA.MsgQueue.push(make_msg(MSG_KEY_DOWN_J));
				case 'K':
				case 'k':
					PlayerA.MsgQueue.push(make_msg(MSG_KEY_DOWN_K));
					break;
				case 'L':
				case 'l':
					PlayerA.MsgQueue.push(make_msg(MSG_KEY_DOWN_L));
					break;
				case 'U':
				case 'u':
					if (PlayerA.FireDelay[6] >= 0)
						PlayerA.MsgQueue.push(make_msg(MSG_KEY_DOWN_U));
					break;
				case 'I':
				case 'i':
					if (PlayerA.FireDelay[7] >= 0)
						PlayerA.MsgQueue.push(make_msg(MSG_KEY_DOWN_I));
					break;
				case ' ':
					PlayerA.MsgQueue.push(make_msg(MSG_KEY_DOWN_SPACE));
					break;
				}
			}
		}
		*/
		for (int i = 0; i < (int)missiles.size(); ++i) {
			missiles[i].Update();
			if (int(missiles[i].x) == PlayerA.x && int(missiles[i].y) == PlayerA.y && missiles[i].fa->id != PlayerA.id) {
				PlayerA.HP -= missiles[i].damage;
				missiles.erase(missiles.begin() + i);
				--i;
				continue;
			}
			else if (int(missiles[i].x) == AI.x && int(missiles[i].y) == AI.y && missiles[i].fa->id != AI.id) {
				AI.HP -= missiles[i].damage;
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
			}
		}
		for (int i = 0; i < (int)lasers.size(); ++i) {
			lasers[i].Update();
			if (lasers[i].liveTime < 0) {
				lasers.erase(lasers.begin() + i);
				--i;
				continue;
			}
		}
		PlayerA.Update();
		AI.Update();
		// SetCursor(1, 1);
		// printf("%d", timeCnt);
		// SetColor(PlayerA.color);
		// SetCursor(0, 1);
		// printf("%3d", PlayerA.HP);
		// SetColor(AI.color);
		// SetCursor(0, 5);
		// printf("%3d", AI.HP);
		/*debug*/
		// SetColor(PlayerA.color);
		// SetCursor(1, 1);
		// printf("%3d", PlayerA.HP * CON_HEIGHT / maxHealth);
		// SetColor(AI.color);
		// SetCursor(1, 5);
		// printf("%3d", AI.HP * CON_HEIGHT / maxHealth);
		/*Health*/
		nowColor = ((PlayerA.color / 16 - 8) * 16);
		for (int i = 0; i < CON_HEIGHT; ++i) {
			if (i >= CON_HEIGHT - PlayerA.HP * CON_HEIGHT / maxHealth) {
				nowColor = (PlayerA.color);
			}
			conDrawBuf[i][0] = nowColor;
		}
		nowColor = ((AI.color / 16 - 8) * 16);
		for (int i = 0; i < CON_HEIGHT; ++i) {
			if (i >= CON_HEIGHT - AI.HP * CON_HEIGHT / maxHealth) {
				nowColor = (AI.color);
			}
			conDrawBuf[i][CON_WIDTH - 1] = nowColor;
		}
		char keymap[15] = "  HJKLUINM";
		for (int i = 1; i < 10; ++i) {
			SetCursor(i + 10, 1);
			if (PlayerA.FireDelay[i] > 0) SetColor((PlayerA.color / 16 - 8) * 16 + 7);
			else SetColor(PlayerA.color);
			printf("%c  %-3d", keymap[i], PlayerA.FireDelay[i]);
			drawed[i + 10][1] = 1;
			drawed[i + 10][2] = 1;
			drawed[i + 10][3] = 1;
		}
		for (int i = 1; i < 10; ++i) {
			SetCursor(i + 10, CON_WIDTH - 4);
			if (AI.FireDelay[i] > 0) SetColor((AI.color / 16 - 8) * 16 + 7);
			else SetColor(AI.color);
			printf("%3d  %c", AI.FireDelay[i], i - 1 + '0');
			drawed[i + 10][CON_WIDTH - 4] = 1;
			drawed[i + 10][CON_WIDTH - 3] = 1;
			drawed[i + 10][CON_WIDTH - 2] = 1;
		}
		++timeCnt;
		int lst = conDrawBuf[0][0];
		SetColor(lst);
		for (int i = 0; i < CON_HEIGHT; ++i) {
			for (int j = 0; j < CON_WIDTH; ++j) {
				if (drawed[i][j]) continue;
				if (conDrawBuf[i][j] == conLstBuf[i][j]) continue;
				SetCursor(i, j);
				if (conDrawBuf[i][j] != lst) {
					lst = conDrawBuf[i][j];
					SetColor(lst);
				}
				putch(' '), putch(' ');
			}
		}
		if (timeCnt % 2 == rand() % 2) {
			AIActive(AI, PlayerA);
		}
		if (timeCnt % 2 == rand() % 2) {
			AIActive(PlayerA, AI);
		}
		memcpy(conLstBuf, conDrawBuf, sizeof(conLstBuf));
		memset(conDrawBuf, 0, sizeof(conDrawBuf));
		memset(drawed, 0, sizeof(drawed));
		// cheat
		// 无CD
		// for (int i = 1; i < 6; ++i) {
		// 	if (PlayerA.FireDelay[i] > 5)
		// 		PlayerA.FireDelay[i] = 5;
		// }
		// for (int i = 6; i < 9; ++i) {
		// 	if (PlayerA.FireDelay[i] > 90)
		// 		PlayerA.FireDelay[i] = 90;
		// }
		// for (int i = 1; i < 6; ++i) {
		// 	if (AI.FireDelay[i] > 5)
		// 		AI.FireDelay[i] = 5;
		// }
		// for (int i = 6; i < 9; ++i) {
		// 	if (AI.FireDelay[i] > 90)
		// 		AI.FireDelay[i] = 90;
		// }
		// // 锁血
		// if (PlayerA.HP < maxHealth) {
		// 	PlayerA.HP += 1;
		// }
		// if (AI.HP < maxHealth) {
		// 	AI.HP += 1;
		// }
		// 回血
		// if (PlayerA.HP < maxHealth) {
		// 	PlayerA.HP = maxHealth - (maxHealth - PlayerA.HP) * 0.9999;
		// }
		// if (AI.HP < maxHealth) {
		// 	AI.HP = maxHealth - (maxHealth - AI.HP) * 0.9999;
		// }
		// Sleep(10);
	}
	while (kbhit()) getch();
	system("color 07 & cls");
	SetCursor(1, 1);
	if (PlayerA.HP > 0) {
		printf("PlayerA Win\n");
	}
	if (AI.HP > 0) {
		printf("AI Win\n");
	}
	Sleep(1000);
	return 0;
}