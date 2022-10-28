#ifndef WINAPI_HPP_
#define WINAPI_HPP_

#include <windows.h>
#include "System.hpp"

// Windows API

#define CON_HEIGHT    40                          // 单位: em
#define CON_WIDTH     40                         // 单位: em

#define CHAR_HEIGHT   16                          // 单位: px
#define CHAR_WIDTH    8                           // 单位: px

extern HANDLE hCon;
extern HWND hwnd;
struct pos {
    int x, y;
};
void SetCursor(pos ps) {
    COORD position;                               // 坐标
    position.X = ps.y;                            // 复制目标位置到坐标变量
    position.Y = ps.x;                            // 同上
    SetConsoleCursorPosition(hCon, position);     // 调用ConsoleAPI设置
}
void SetCursor(int x, int y) {
    COORD position;                               // 坐标
    position.X = y * 2;                               // 复制目标位置到坐标变量
    position.Y = x;                               // 同上
    SetConsoleCursorPosition(hCon, position);     // 调用ConsoleAPI设置
}

void GetCursor(pos &ps) {
    POINT pt;                                     // 点
    GetCursorPos(&pt);                            // 获取鼠标所在的点
    ScreenToClient(hwnd, &pt);                    // 将绝对坐标换算为窗口相对坐标
    ps.x = pt.y / CHAR_HEIGHT;                    // 换算成控制台坐标
    ps.y = pt.x / CHAR_WIDTH / 2;                     // 同上
}
void HideCursor() {
    CONSOLE_CURSOR_INFO cursor_info = {1, 0};     // {光标厚度 1~100%, 是否可见 0/1}
    SetConsoleCursorInfo(hCon, &cursor_info);     // 调用ConsoleAPI
}
void SetColor(int color) {
    SetConsoleTextAttribute(hCon, color);         // 调用ConsoleAPI
}

#endif
