
#ifndef RE2048_CONIO_LINUX_H
#define RE2048_CONIO_LINUX_H
#endif //RE2048_CONIO_LINUX_H

#include <termios.h>
#include <unistd.h>
#include <stdio.h>

int getch(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr(STDIN_FILENO, &oldattr);
    newattr = oldattr;
    newattr.c_lflag &= ~(0u | ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    return ch;
}

void clean_stdin()
{
    int c = 0;
    while (c != '\n' && c != EOF)
        c = getchar();
}