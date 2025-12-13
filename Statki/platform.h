#pragma once
#include <iostream>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

#ifndef _ships_
    #define _ships_
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
