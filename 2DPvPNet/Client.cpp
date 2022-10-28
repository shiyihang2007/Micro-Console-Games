#include <winsock2.h>

#include "ConDraw.hpp"
#include "WinAPI.hpp"
#include "System.hpp"

#include <cmath>
#include <ctime>
#include <string>

// #pragma comment(lib, "ws2_32.lib")
using namespace std;

char recData[2500005];
int conDrawBuf[505][505];
int conLstBuf[505][505];
int timeCnt;

string getkey() {
    string res = "";
    for (int i = 1; i < 127; ++i) {
        if (MKEY_DOWN(i)) {
            res += char(i);
        }
    }
    res += '\0';
    return res;
}

int main()
{
    srand(time(NULL));
    Sleep(10);
    system("mode con cols=80 lines=40");
    system("mode con rate=40 delay=0");
    system("title Client v0.2");
    hwnd = GetConsoleWindow();
    hCon = GetStdHandle(STD_OUTPUT_HANDLE);
    HideCursor();
    /*加载网络服务*/
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA data;
    if (WSAStartup(sockVersion, &data) != 0) {
        return 0;
    }
    char address[20] = "";
    printf("Input IP address: ");
    scanf("%s", address);
    system("cls");
    int clsFlag = 1;
    while (true) {
        ++timeCnt;
        if (timeCnt % 1000 == 0) {
            system("color 07");
            memset(conLstBuf, 0, sizeof(conLstBuf));
        }
        SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (sclient == INVALID_SOCKET)
        {
            clsFlag = 1;
            printf("invalid socket!");
            // return 0;
            continue;
        }
        /*连接服务端*/
        sockaddr_in serAddr;
        serAddr.sin_family = AF_INET;
        serAddr.sin_port = htons(25565);
        serAddr.sin_addr.S_un.S_addr = inet_addr(address);
        if (connect(sclient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR) {   //连接失败
            clsFlag = 1;
            printf("connect error!\n");
            closesocket(sclient);
            // return 0;
            continue;
        }
        const char* sendData;
        // int tot = 0, ch = 0;
        /*while (kbhit()) {
            ch = getch();
            if (ch >= 128) {
                sendData[tot++] = ch % 128, sendData[tot++] = ch / 128;
            }
            else {
                sendData[tot++] = ch, sendData[tot++] = 0;
            }
            // putch('!');
        }*/
        string inputData = getkey();
        sendData = inputData.c_str();
        // putch('#');
        send(sclient, sendData, inputData.size(), 0);
        // putch('#');
        int ret = recv(sclient, recData, 1600, 0);
        // putch('#');
        if (clsFlag) {
            system("color 07 & cls");
            clsFlag = 0;
        }
        if (ret > 0) {
            recData[ret] = 0x00;
            for (int i = 0; i < 1600; ++i) {
                SetCursor(i / 40, i % 40);
                // printf("%2d", recData[i]);
                conDrawBuf[i / 40][i % 40] = recData[i] * 16;
            }
        }
        // putch('#');
        int lst = conDrawBuf[0][0];
        SetColor(lst);
        for (int i = 0; i < CON_HEIGHT; ++i) {
            for (int j = 0; j < CON_WIDTH; ++j) {
                if (conDrawBuf[i][j] == conLstBuf[i][j]) continue;
                if (conDrawBuf[i][j] != lst) {
                    lst = conDrawBuf[i][j];
                    SetColor(lst);
                }
                SetCursor(i, j);
                putch(' '), putch(' ');
            }
        }
        memcpy(conLstBuf, conDrawBuf, sizeof(conLstBuf));
        memset(conDrawBuf, 0, sizeof(conDrawBuf));
        Sleep(5);
        closesocket(sclient);
    }
    WSACleanup();
    return 0;

}