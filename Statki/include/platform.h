#pragma once
#include <iostream>
#include <cstdlib>
#ifdef _WIN32
    #include <windows.h>
    #include <conio.h>
#else
    #include <unistd.h>
    #include <termios.h>
#endif

inline void sleepMs(int ms) 
{
    #ifdef _WIN32
        Sleep(ms);
    #else
        usleep(ms * 1000);
    #endif
}
inline void clearScreen()
{
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}
inline char getKey()
{
    #ifdef _WIN32
    {
        return _getch();
    }
    #else
    {
        std::string c;
        std::cin >> c;
        return c[0];
    }
    #endif
}