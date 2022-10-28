#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <algorithm>
#include <queue>
#include <conio.h>

#ifndef _SYSTEM_HPP_
#define _SYSTEM_HPP_

// system

HANDLE hCon;
HWND hwnd;

// 224
#define KEY_HOME  71
#define KEY_UP    72
#define KEY_PAGEU 73
#define KEY_LEFT  75
#define KEY_RIGHT 77
#define KEY_END   79
#define KEY_DOWN  80
#define KEY_PAGED 81
#define KEY_INS   82
#define KEY_DEL   83
#define KEY_F11   113
#define KEY_F12   134
// 0
#define KEY_F1  59
#define KEY_F2  60
#define KEY_F3  61
#define KEY_F4  62
#define KEY_F5  63
#define KEY_F6  64
#define KEY_F7  65
#define KEY_F8  66
#define KEY_F9  67
#define KEY_F10 68
// Other
#define KEY_ESC 27

#define CON_COLOR_DARK_BLACK    0
#define CON_COLOR_DARK_BLUE     1
#define CON_COLOR_DARK_GREEN    2
#define CON_COLOR_DARK_CYAN     3
#define CON_COLOR_DARK_RED      4
#define CON_COLOR_DARK_PURPLE   5
#define CON_COLOR_DARK_YELLOW   6
#define CON_COLOR_DARK_WHITE    7
#define CON_COLOR_LIGHT_BLACK   8
#define CON_COLOR_LIGHT_BLUE    9
#define CON_COLOR_LIGHT_GREEN   10
#define CON_COLOR_LIGHT_CYAN    11
#define CON_COLOR_LIGHT_RED     12
#define CON_COLOR_LIGHT_PURPLE  13
#define CON_COLOR_LIGHT_YELLOW  14
#define CON_COLOR_LIGHT_WHITE   15

#define CON_COLOR(CON_BACKGROUD_COLOR,CON_FOREGROUD_COLOR) ((CON_BACKGROUD_COLOR)*16+(CON_FOREGROUD_COLOR))

#define MKEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1 : 0)

#define MSGTYPE unsigned long

#define MSG_QUIT    0
#define MSG_CLOSE   1

#define MSG_ESC     101
#define MSG_MUNE    102

#define MSG_KEY_DOWN_A 201
#define MSG_KEY_DOWN_B 202
#define MSG_KEY_DOWN_C 203
#define MSG_KEY_DOWN_D 204
#define MSG_KEY_DOWN_E 205
#define MSG_KEY_DOWN_F 206
#define MSG_KEY_DOWN_G 207
#define MSG_KEY_DOWN_H 208
#define MSG_KEY_DOWN_I 209
#define MSG_KEY_DOWN_J 210
#define MSG_KEY_DOWN_K 211
#define MSG_KEY_DOWN_L 212
#define MSG_KEY_DOWN_M 213
#define MSG_KEY_DOWN_N 214
#define MSG_KEY_DOWN_O 215
#define MSG_KEY_DOWN_P 216
#define MSG_KEY_DOWN_Q 217
#define MSG_KEY_DOWN_R 218
#define MSG_KEY_DOWN_S 219
#define MSG_KEY_DOWN_T 220
#define MSG_KEY_DOWN_U 221
#define MSG_KEY_DOWN_V 222
#define MSG_KEY_DOWN_W 223
#define MSG_KEY_DOWN_X 224
#define MSG_KEY_DOWN_Y 225
#define MSG_KEY_DOWN_Z 226

#define MSG_KEY_DOWN_SPACE 227

struct msg {
	int type;
	int x, y;
	msg(MSGTYPE tp = 0, int a = 0, int b = 0) : type(tp), x(a), y(b) {}
};
msg make_msg(MSGTYPE type, int x = 0, int y = 0) {
	msg tmp;
	tmp.type = type;
	tmp.x = x;
	tmp.y = y;
	return tmp;
}

#endif
