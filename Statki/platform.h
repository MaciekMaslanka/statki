#pragma once

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

#ifndef _ships_
    #define _ships_
#endif

inline void sleep_ms(int ms) 
{
    #ifdef _WIN32
        Sleep(ms);
    #else
        usleep(ms * 1000);
    #endif
}
