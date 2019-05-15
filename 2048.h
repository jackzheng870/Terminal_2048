// Terminal 2048
// C
// CodeNemo (GitHub)
// Version 1.4
// Tested: Windows MinGW, Debian GCC, macOS

#ifndef TERMINAL2048_2048_H
#define TERMINAL2048_2048_H
#endif //TERMINAL2048_2048_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>
#include <ctype.h>

// on Windows
#if defined(WIN32) || defined(WIN64)
    #include <windows.h>
    #include <conio.h>
    #define clear system("cls")
    #define pause(sec) Sleep(1000*sec)
    #define flush fflush(stdin)
// on UNIX and UNIX-like
#elif defined(unix) || defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))
    #include "conio-linux.h"
    #define flush clean_stdin()
    #define pause(sec) usleep(1000000*sec)
    #define clear system("clear")
// other systems
#else
    #error "Not supported clear"
#endif
