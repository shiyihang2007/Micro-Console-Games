#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <ctime>
#include <queue>
#include <windows.h>
using namespace std;
// #define _DEBUG_
int n, m;
int x0, y0, x1, y1;
int mp[1005][1005];
int tmp[1005][1005];
struct node{
	node* pre;
	int x, y;
	int step;
	double away;
	friend bool operator<(const node &a, const node &b) {
		// if (abs(a.away - b.away) < 0.001) {
			return a.step - b.step;
		// }
		// return a.away - b.away;
	}
} v[1000005];
int tot = 0;
node make_node(double away, int step, int x, int y, node* pre = NULL) {
	node tmp;
	tmp.away = away;
	tmp.x = x, tmp.y = y;
	tmp.step = step;
	tmp.pre = pre;
	return tmp;
}
/**
 * 随机撒落器
 * @author: ShiYihang
 * @version: v0.2.1
 */
void makeMap() {
	while (abs(x0 - y0) + abs(x1 - y1) < 50) {
		x0 = rand() % n, y0 = rand() % m;
		x1 = rand() % n, y1 = rand() % m;
	}
	memset(mp, 0, sizeof(mp));
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			if (rand() % 2 == 0)
				mp[i][j] = 0;
			else
				mp[i][j] = 1;
		}
	}
	mp[x0][y0] = 0, mp[x1][y1] = 0;
}
/**
 * 导出地图
 * @author: ShiYihang
 * @version: 0.1.0
 */
void printMap() {
	printf("%d %d \n", n, m);
	printf("%d %d %d %d\n", x0, y0, x1, y1);
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			if (i == x0 && j == y0)
				putchar('O');
			else if (i == x1 && j == y1)
				putchar('@');
			else if (mp[i][j] == 0)
				putchar(' ');
			else
				putchar('#');
		}
		putchar('\n');
	}
}
/**
 * 元胞自动机_前置_计算周围细胞数
 * @author: ShiYihang
 * @version: 0.1.0
 * @param: x 该细胞所在的行
 * @param: y 该细胞所在的列
 * @return: 该细胞周围的细胞数
 */
int clar(int x, int y) {
	int sum = 0;
	for (int i = -1; i <= 1; ++i) {
		for (int j = -1; j <= 1; ++j) {
			if (i == 0 && j == 0) {
				continue;
			}
			sum += mp[x + i][y + j];
		}
	}
	// printf("(%d,%d) %d\n", x, y, sum);
	return sum;
}
/**
 * 元胞自动机
 * @author: ShiYihang
 * @version: 0.1.0
 */
void cell() {
	for (int i = 1; i < n - 1; ++i) {
		for (int j = 1; j < m - 1; ++j) {
			if (clar(i, j) <= 3 || clar(i, j) >= 7) tmp[i][j] = 0;
			else tmp[i][j] = 1;
		}
	}
	for (int i = 1; i < n - 1; ++i) {
		for (int j = 1; j < m - 1; ++j) {
			mp[i][j] = tmp[i][j];
		}
	}
}
int vis[1005][1005];
int w[][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}}; 
int aStar() {
	memset(vis, 0, sizeof(vis));
	tot = 0;
	priority_queue<node> q;
	q.push(make_node(sqrt(pow(abs(x0 - x1), 2) + pow(abs(y0 - y1), 2)), 0, x0, y0));
	while (!q.empty()) {
		int away = q.top().away, x = q.top().x, y = q.top().y, step = q.top().step;
		v[++tot] = q.top();
		if (tot > 1000000) {
			return 0;
		}
		node* now = &v[tot];
		q.pop();
		if (x == x1 && y == y1) {
			int cnt = 0;
			while (now->pre != NULL) {
				mp[now->x][now->y] = 0;
				now = now->pre;
				++cnt;
				printf("info # %2.2lf %3d (%2d, %2d) \n", now->away, now->step, now->x, now->y);
			}
			printf("info total=%-4d\n", cnt);
			return cnt;
		}
		for (int i = 0; i < 4; ++i) {
			int tx = x + w[i][0], ty = y + w[i][1];
			double taw = sqrt(pow(abs(tx - x1), 2) + pow(abs(ty - y1), 2));
			if (tx >= 0 && tx < n && ty >= 0 && ty < m && (!vis[tx][ty] || vis[tx][ty] > step + 15)) {
				if (mp[tx][ty]) {
					vis[tx][ty] = step + 10;
					q.push(make_node(taw, step + 10, tx, ty, now));
				}
				else {
					vis[tx][ty] = step + 1;
					q.push(make_node(taw, step + 1, tx, ty, now));
				}
			}
		}
		// if (x > 0 && !mp[x - 1][y] && (!vis[x - 1][y] || vis[x - 1][y] > step))
		// 	q.push(make_pair(abs(x - 1 - x1) + abs(y - y1), make_pair(make_pair(x - 1, y), step + 1)));
		// if (x < n - 1 && !mp[x + 1][y] && (!vis[x + 1][y] || vis[x + 1][y] > step))
		// 	q.push(make_pair(abs(x + 1 - x1) + abs(y - y1), make_pair(make_pair(x + 1, y), step + 1)));
		// if (y > 0 && !mp[x][y - 1] && (!vis[x][y - 1] || vis[x][y - 1] > step))
		// 	q.push(make_pair(abs(x - x1) + abs(y - 1 - y1), make_pair(make_pair(x, y - 1), step + 1)));
		// if (y < m - 1 && !mp[x][y + 1] && (!vis[x][y + 1] || vis[x][y + 1] > step))
		// 	q.push(make_pair(abs(x - x1) + abs(y + 1 - y1), make_pair(make_pair(x, y + 1), step + 1)));
	}
	return -1;
}
int main(int argc, char const *argv[])
{
	int t = 0, difficult = 0;
	if (argc > 1) difficult = atoi(argv[1]);
	if (argc >= 2) srand(time(NULL) + atoi(argv[2]));
	else srand(time(NULL));
#ifndef _DEBUG_
	freopen("mapmake.log", "w", stdout);
#endif
	printf("info difficult=%d\n", difficult);
	n = rand() % 30 + 20;
	m = rand() % 30 + 20;
	do {
		makeMap();
		cell();
		printMap();
	} while (aStar() < difficult);
	printMap();
#ifndef _DEBUG_
	fclose(stdout);
	freopen("CON", "w", stdout);
#endif
	freopen("map.txt", "w", stdout);
	printMap();
	fclose(stdout);
	freopen("CON", "w", stdout);
	return 0;
}