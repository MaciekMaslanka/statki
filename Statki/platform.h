#pragma once

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

inline void sleep(int ms) 
{
#ifdef _WIN32
    Sleep(ms);
#else
    sleep(ms);
#endif
}
