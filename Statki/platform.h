#pragma once
#include <iostream>
#include <cstdlib>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
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