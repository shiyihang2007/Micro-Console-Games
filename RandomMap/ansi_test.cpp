#include <cstdio>
#include <conio.h>
using namespace std;
int main(int argc, char const *argv[])
{
	while (1) {
		while (kbhit()) {
			int ch = getch();
			printf("%d %c\n", ch, ch);
		}
	}
	return 0;
}